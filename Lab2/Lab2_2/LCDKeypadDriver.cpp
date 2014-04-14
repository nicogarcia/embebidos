#include "Arduino.h"
#include "LCDKeypadDriver.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// Singleton instance allocation
LCDKeypadDriver LCDKeypadDriver::instance;

static bool initiated = false;
// Singleton instance access method
LCDKeypadDriver* LCDKeypadDriver::Instance(){
	if(!initiated){
		instance = LCDKeypadDriver();
		initiated = true;
	}
	return &instance;
}

// Contructor
LCDKeypadDriver::LCDKeypadDriver() {
	event_flag = false;

	timer_enabled = false;

	for(int i = 0; i < LCDKeypadKeys::KEY_COUNT; i++){
		callbacks[KEY_UP_CALLBACK][i] = NULL;
		callbacks[KEY_DOWN_CALLBACK][i] = NULL;
	}

	adc_initialize();
}

// TOREV
void LCDKeypadDriver::adc_initialize(){
	// Based on example found in http://maxembedded.com/2011/06/20/the-adc-of-the-avr/

	// AREF = AVcc
	//1 shift 6 places (01000000) to define the reference for the ADC. In this case 5V
	//ADLAR bit is set 0 in ADMUX register because we want the high part of the conversion
	// in ADCH register.
	ADMUX = (1<<REFS0);
	
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	/**
	 * ADEN: ADC Enable set to 1 to enable de ADC
	 * ADIE: ADC Interrupt Enable. Set to 1 to enable de ADC interrupts
	 * ADPS2::0: The prescalar is define by this three bits. 111 means 128.
	 **/
	ADCSRA = (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	sei(); // Necessary?
	
	ADCSRA |= 1<<ADSC;	// Start Conversion
}

// Registers the callback function for the keydown event of the corresponding key
void LCDKeypadDriver::registerOnKeyDownCallback(void (*handler)(), int key){
	callbacks[KEY_DOWN_CALLBACK][key] = handler;
}

// Registers the callback function for the keyup event of the corresponding key
void LCDKeypadDriver::registerOnKeyUpCallback(void (*handler)(), int key){
	callbacks[KEY_UP_CALLBACK][key] = handler;
}

// Checks if there's a function to be called
// It should be used in the main loop to check if there's a function to
// call, note that this is made outside of the ISR
void LCDKeypadDriver::checkEvents(){
	if(event_flag){
		function_to_be_called();
		event_flag = false;
	}
}

// TOREV
void LCDKeypadDriver::init_debouncing_timer(){
	// Initialize timer1
	noInterrupts();           // Disable all interrupts

	timer_enabled = true;	  // Set timer as enabled

	TCCR2A = 0;				  // TCCR1A isn't necessary
	TCCR2B = 0;
	TCNT2  = 0;				  // Initialize counter

	OCR2A = 255;              // Compare match register 16MHz/64/20Hz
	TCCR2B |= (1 << WGM22);   // CTC mode
	TCCR2B |= (1 << CS22);	  // 1024 prescaler
	TCCR2B |= (1 << CS21);    // We need to count ~50ms so,
	TCCR2B |= (1 << CS20);    // 16MHz/1024 = 15,62KHz ~> 64us per cycle,
							  // and 50k/64 = 781,25 cycles)
	TIMSK2 |= (1 << OCIE2A);  // Enable timer compare interrupt
	interrupts();             // Enable all interrupts
}

/************************************************************************
	Code to handle interrupts
	TIMER1_COMPA_vect y ADC_vect are LCDKeypadDriver 'friends' so
	they have access to its members (like callbacks array, defines, etc)
************************************************************************/

// Timer ISR variables
volatile const int count_max = 3; // approx 50ms with 1024 prescaler
volatile int count_qty = 0; // counts the ticks of the timer
volatile int key_read_before_timer = -1;
volatile int callback_type_before_timer = -1;

// Key ISR variables
volatile int last_key = -1;
volatile int current_key = -1;
volatile int callback_type = -1;
volatile int key_involved = -1;

// Timer ISR
void TIMER2_COMPA_vect(){
	// Test 2nd level timer against its max
	if(++count_qty < count_max)
		return;

	// Reset 2nd level timer counter
	count_qty = 0;

	LCDKeypadDriver* kpd = LCDKeypadDriver::Instance();
	
	// If the key read before timer is the same as the current
	if(key_read_before_timer == key_involved &&
		callback_type == callback_type_before_timer){
		// And there's a callback registered then set callback to be executed.
		if(kpd->callbacks[callback_type][key_involved] != NULL){
			kpd->function_to_be_called = kpd->callbacks[callback_type][key_involved];
			kpd->event_flag = true;
		}
	}
	
	// Disable timer counter interrupt when the values match.
	TIMSK2 ^= (1 << OCIE2A);

	// Set timer as disabled
	kpd->timer_enabled = false;
}

// Key ISR
void ADC_vect(){
	
	// Read digital value from ADC register
	current_key = LCDKeypadKeys::GetKey(ADC);
	
	// Alias KeyPadDriver singleton for simplicity
	LCDKeypadDriver* kpd = LCDKeypadDriver::Instance();
	
	// Test if there were changes (keyup or keydown)
	if(last_key != current_key){
		// Toggle led when there's a change in state for debug
		PORTB ^= (1 << PB5);

		// key_involved stores the key being pushed or released
		// to access the callbacks array
		key_involved = (last_key == -1) ? current_key : last_key;

		// Use last_key to set if there was keyup or keydown
		// and store it in callback_type for direct access
		callback_type = (last_key == -1) ? KEY_DOWN_CALLBACK : KEY_UP_CALLBACK;
		
		// If not started, start debouncing timer
		if(!kpd->timer_enabled){
			// Store read key and callback type before starting debouncing timer
			key_read_before_timer = key_involved;
			callback_type_before_timer = callback_type;
			kpd->init_debouncing_timer();
		}
		
		// Update last key
		last_key = current_key;
	}
	
	// Start a new conversion
	ADCSRA |= 1<<ADSC;
}