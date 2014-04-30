#include "TempSensorDriver.h"
#include "SystemClock.h"

TempSensorDriver_ lm35;

// FIXME: Should not be static, to be removed
double TempSensorDriver_::temperature;

void TempSensorDriver_::driver_ISR_temp( int value ) {
    TempSensorDriver_::temperature = (110 * value) / 1024.0;
    PORTB ^= (1 << PB5);
    SystemClock.enqueue(lm35.callback);
}

void TempSensorDriver_::initialize() {
    time = 130;
    enabled = true;
    isr_func = driver_ISR_temp;
    analog_reference = REF_INTERNAL_1V1;
}
