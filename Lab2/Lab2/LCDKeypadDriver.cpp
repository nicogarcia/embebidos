#include "Arduino.h"
#include <avr/io.h>
#include "LCDKeypadDriver.h"
#include <avr/interrupt.h>

// Singleton instance allocation
LCDKeypadDriver* LCDKeypadDriver::instance = NULL;

// Singleton instance access method
LCDKeypadDriver* LCDKeypadDriver::Instance(){
	return instance ? instance : (instance = new LCDKeypadDriver());
}

// Contructor
LCDKeypadDriver::LCDKeypadDriver() {
	event_flag = false;

	for(int i = 0; i < LCDKeypadKeys::KEY_COUNT; i++){
		callbacks[KEY_UP_CALLBACK][i] = NULL;
		callbacks[KEY_DOWN_CALLBACK][i] = NULL;
	}

	adc_initialize();

	//init_debouncing_timer();
}

// TOREV
void LCDKeypadDriver::adc_initialize(){
	// AREF = AVcc
	ADMUX = (1<<REFS0);
	
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	sei();
	
	ADCSRA |= 1<<ADSC;	// Start Conversion
}

void LCDKeypadDriver::registerOnKeyDownCallback(void (*handler)(), int key){
	callbacks[KEY_DOWN_CALLBACK][key] = handler;
}

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
	// initialize timer1
	noInterrupts();           // disable all interrupts
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;

	OCR1A = 31250;            // compare match register 16MHz/256/2Hz
	TCCR1B |= (1 << WGM12);   // CTC mode
	TCCR1B |= (1 << CS12);    // 256 prescaler
	TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
	interrupts();             // enable all interrupts
}

/************************************************************************
	C code to handle interrupts
	TIMER1_COMPA_vect y ADC_vect are LCDKeypadDriver 'friends' so
	they have access to its members (like callbacks array, defines, etc)
************************************************************************/

// Timer ISR variables
volatile int key_read_before_timer;

// Key ISR variables
volatile int last_key = -1;
volatile int current_key = -1;
volatile int callback_type = -1;
volatile int key_involved = -1;

// Timer ISR
void TIMER1_COMPA_vect(){
	LCDKeypadDriver* kpd = LCDKeypadDriver::Instance();
	
	if(current_key == key_read_before_timer)
		if(kpd->callbacks[callback_type][current_key] != NULL){
			kpd->function_to_be_called = kpd->callbacks[callback_type][current_key];
			kpd->event_flag = true;
		}
}

// Key ISR
void ADC_vect(){
	
	// Read digital value from ADC register
	current_key = LCDKeypadKeys::GetKey(ADC);
	
	// Alias KeyPadDriver singleton for simplicity
	LCDKeypadDriver* kpd = LCDKeypadDriver::Instance();
	
	// Test if there were changes (keyup or keydown)
	if(last_key != current_key){		
		// Use last_key to set if there was keyup or keydown
		// and store it in callback_type for direct access
		callback_type = (last_key == -1) ? KEY_DOWN_CALLBACK : KEY_UP_CALLBACK;
		
		// key_involved stores the key being pushed or released
		// to access the callbacks array
		key_involved = (last_key == -1) ? current_key : last_key;
		
		// Store read key before programming debouncing timer
		key_read_before_timer = current_key;
		// TODO: Program debouncing timer
		
		// Set callback to be called from main loop (this should go in Timer ISR)		
		if(kpd->callbacks[callback_type][key_involved] != NULL){
			kpd->function_to_be_called = kpd->callbacks[callback_type][key_involved];
			kpd->event_flag = true;
		}

		// Update last key
		last_key = current_key;
	}
	
	// Start a new conversion
	ADCSRA |= 1<<ADSC;
}