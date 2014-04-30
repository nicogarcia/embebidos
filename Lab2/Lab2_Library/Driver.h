#ifndef DRIVER_H_
#define DRIVER_H_

#include "Arduino.h"
#include "SystemClock.h"

class Driver {

public:
    Driver();

    fptr callback;

    void (*isr_func)(int);

    //The function to do
    void driver_ISR(int adc_value);

    //virtual void initialize();

    //the time between execution of the function
    unsigned long time;

    //the last time the function was execute
    unsigned long last_execution;

    enum REFERENCES {
        REF_DEFAULT = 1,
        REF_INTERNAL_1V1 = 3
    };
    // Set analog reference
    byte analog_reference;

    //is null the driver?
    bool enabled;

private:

};

#endif /* DRIVER_H_ */