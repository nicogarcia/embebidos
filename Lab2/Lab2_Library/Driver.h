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

    //is null the driver?
    bool enable;

private:

};

#endif /* DRIVER_H_ */