#ifndef DRIVER_H_
#define DRIVER_H_

#include "Arduino.h"
#include "SystemClock.h"

typedef void (*fptr)();

class Driver {

public:
    Driver();
    //constructor for this class
    Driver(fptr funct, unsigned long every_time);
    //The function to do
    fptr function;
    //the time between execution of the function
    unsigned long time;
    //the last time the function was execute
    unsigned long last_execution;
    //The value of the conversion
    int value;
    //To execute the function and update the values
    void execute_function();
    //is null the driver?
    bool enable;

private:

};

#endif /* DRIVER_H_ */