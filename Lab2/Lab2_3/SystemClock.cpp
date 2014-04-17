#include "Arduino.h"
#include "SystemClock.h"
#include <avr/io.h>
#include <avr/interrupt.h>

SystemClock_ SystemClock;

SystemClock_::SystemClock_() {
    millis = 0;
    callback = NULL;
    time_ms = 0;
    start_time = 0;
    event_flag = false;
}

// Get system millis
unsigned long SystemClock_::getMillis() {
    return millis;
}

// Initialize TIMER0 parameters
void SystemClock_::init() {
    // Initialize timer0
    noInterrupts();           // Disable all interrupts

    TCCR0A = 0;				  // TCCR1A isn't necessary
    TCCR0B = 0;
    TCNT0  = 0;				  // Initialize counter

    OCR0A = 249;              // Compare match register 16MHz/64/20Hz
    TCCR0B |= (1 << WGM02);   // CTC mode

    TCCR0B |= (1 << CS01);    // We need to count 1ms so...
    TCCR0B |= (1 << CS00);	  // 64 prescaler

    TIMSK0 |= (1 << OCIE0A);  // Enable timer compare interrupt
    interrupts();             // Enable all interrupts
}

// Attach callback
void SystemClock_::attach(void (*callback)(), long time_ms) {
    start_time = getMillis();
    this->time_ms = time_ms;
    this->callback = callback;
}

// Check if there's a callback to call
void SystemClock_::checkEvents() {
    if(callback != NULL && event_flag) {
        callback();
        event_flag = false;
    }
}

// Timer ISR to count millis
void TIMER0_COMPA_vect() {
    SystemClock.millis++;
    if(SystemClock.callback != NULL) {
        if(SystemClock.millis - SystemClock.start_time >= SystemClock.time_ms) {
            SystemClock.start_time = SystemClock.millis;
            SystemClock.event_flag = true;
        }
    }
}