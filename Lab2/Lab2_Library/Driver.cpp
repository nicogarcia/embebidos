#include "Driver.h"

Driver::Driver () {
    time = 0;
    last_execution = 0;
    callback = NULL;
    enable = false;
}

void Driver::driver_ISR( int adc_value ) {
    //Serial.print("messi");
    //PORTB ^= (1 << PB5);
    isr_func(adc_value);
}

/*
Driver::Driver(fptr_int funct, unsigned long every_time) {
    time = every_time;
    function = funct;
    last_execution = 0;
    enable = true;
}
*/