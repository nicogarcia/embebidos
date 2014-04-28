#include "TempSensorDriver.h"
#include "SystemClock.h"

void TempSensorDriver_::setTemperature( int value ) {
    temperature = (5.0 * value * 100.0) / 1024;

    SystemClock.enqueue(callback);
}
