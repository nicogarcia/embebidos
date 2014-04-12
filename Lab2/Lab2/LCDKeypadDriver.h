#ifndef LCDKEYPAD_DRIVER_H
#define LCDKEYPAD_DRIVER_H

#include <avr/interrupt.h>
// Include the keyboard driver interface
#include "KeyboardDriver.h"
// Include special properties of the keypad
#include "LCDKeypadKeys.h"

// Callback function array parameters
#define CALLBACKS_DEPTH		2
#define KEY_UP_CALLBACK		0
#define KEY_DOWN_CALLBACK	1

// ISR C++ mapping
extern "C" void TIMER1_COMPA_vect(void) __attribute__ ((signal));
extern "C" void ADC_vect(void) __attribute__ ((signal));

// Implement LCDKeypadDriver for the LCDKeypad Shield
class LCDKeypadDriver : public KeyboardDriver {
	private:
	// Singleton
	LCDKeypadDriver();
	LCDKeypadDriver(LCDKeypadDriver const&);
	const LCDKeypadDriver& operator=(LCDKeypadDriver const&);
	static LCDKeypadDriver* instance;

	// Array of function pointers that registers callbacks
	void (*callbacks[CALLBACKS_DEPTH][LCDKeypadKeys::KEY_COUNT])();

	// Set if there's a callback to call
	bool event_flag;

	// Function to be called if event_flag is set
	// when control is returned to the main
	// loop through LCDKeypadDriver::Instance->checkEvents()
	void (*function_to_be_called)();

	// FIXME
	void init_debouncing_timer();
	void adc_initialize();

	public:
	// Singleton instance access method
	static LCDKeypadDriver* Instance();

	// Function to be used from
	void checkEvents();

	// Unimplemented virtual methods, java equivalent of "interface"
	void registerOnKeyDownCallback(void (*handler)(), int key);
	void registerOnKeyUpCallback(void (*handler)(), int key);

	// ADC ISR
	friend void ADC_vect(void);
	// Timer ISR
	friend void TIMER1_COMPA_vect(void);
};

#endif
