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
extern "C" void ADC_vect(void) __attribute__ ((signal));

// Implement LCDKeypadDriver for the LCDKeypad Shield
class LCDKeypadDriver : public KeyboardDriver {
public:
    LCDKeypadDriver();

    // Unimplemented virtual methods, java equivalent of "interface"
    void registerOnKeyDownCallback(void (*handler)(), int key);
    void registerOnKeyUpCallback(void (*handler)(), int key);

private:
    // Array of function pointers that registers callbacks
    void (*callbacks[CALLBACKS_DEPTH][LCDKeypadKeys::KEY_COUNT])();

    // Set if there's a callback to call
    bool event_flag;

    // Function to be called if event_flag is set
    // when control is returned to the main
    // loop through LCDKeypadDriver::Instance->checkEvents()
    void (*function_to_be_called)();

    // Initializes the ADC
    void adc_initialize();

    // ADC ISR
    friend void ADC_vect(void);
};
extern LCDKeypadDriver KeypadDriver;

#endif
