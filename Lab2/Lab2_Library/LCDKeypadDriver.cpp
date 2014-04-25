#include "Arduino.h"
#include "LCDKeypadDriver.h"
#include "SystemClock.h"
#include <avr/io.h>
#include <avr/interrupt.h>

LCDKeypadDriver KeypadDriver;

// Contructor
LCDKeypadDriver::LCDKeypadDriver() {
    event_flag = false;

    for(int i = 0; i < LCDKeypadKeys::KEY_COUNT; i++) {
        callbacks[KEY_UP_CALLBACK][i] = NULL;
        callbacks[KEY_DOWN_CALLBACK][i] = NULL;
    }

    adc_initialize();
}

// ADC initialization routine
void LCDKeypadDriver::adc_initialize() {
    // Based on example found in http://maxembedded.com/2011/06/20/the-adc-of-the-avr/
    noInterrupts();

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

    interrupts();

    ADCSRA |= 1<<ADSC;	// Start Conversion
}

// Registers the callback function for the keydown event of the corresponding key
void LCDKeypadDriver::registerOnKeyDownCallback(void (*handler)(), int key) {
    callbacks[KEY_DOWN_CALLBACK][key] = handler;
}

// Registers the callback function for the keyup event of the corresponding key
void LCDKeypadDriver::registerOnKeyUpCallback(void (*handler)(), int key) {
    callbacks[KEY_UP_CALLBACK][key] = handler;
}

/************************************************************************
	Code to handle interrupts
	TIMER1_COMPA_vect y ADC_vect are LCDKeypadDriver 'friends' so
	they have access to its members (like callbacks array, defines, etc)
************************************************************************/

// Key ISR variables
volatile int last_key = -1;
volatile int current_key = -1;
volatile int callback_type = -1;
volatile int key_involved = -1;
volatile int last_key_before_db = -1;
volatile bool finish_debouncing = false;
volatile bool bouncing = false;

//  Debouncing callback
void debouncing() {
    finish_debouncing = true;
    bouncing = false;
}

// FIXME: TO BE REVIEWED: TIMER SHOULD BE STARTED ON CHANGE, ELSE ADC SHOULD KEEP CONVERTING!
// Key ISR
void ADC_vect() {

    // Read digital value from ADC register
    current_key = LCDKeypadKeys::GetKey(ADC);

    // Test if there were changes (keyup or keydown)
    if(last_key != current_key) {
        // Toggle led when there's a change in state (for debugging)
        PORTB ^= (1 << PB5);

        if(!bouncing) {
            last_key_before_db = last_key;
            // Wait for timer to launch the next ADC conversion after debouncing period
            SystemClock.attach(Task(30, debouncing));
            bouncing = true;
        }

        // Update last key
        last_key = current_key;
    }

    if(finish_debouncing) {
        // key_involved stores the key being pushed or released
        // to access the callbacks array
        key_involved = (last_key_before_db == -1) ? current_key : last_key_before_db;

        // Use last_key to set if there was keyup or keydown
        // and store it in callback_type for direct access
        callback_type = (last_key_before_db == -1) ? KEY_DOWN_CALLBACK : KEY_UP_CALLBACK;

        // If there's a callback for current key and action, set its callback to be launched
        if(KeypadDriver.callbacks[callback_type][key_involved] != NULL)
            SystemClock.enqueue(KeypadDriver.callbacks[callback_type][key_involved]);

        finish_debouncing = false;
    }

    ADCSRA |= 1<<ADSC;	// Start new ADC conversion
}