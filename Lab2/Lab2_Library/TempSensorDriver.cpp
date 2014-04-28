#include "TempSensorDriver.h"


void TemSensorDriver::setTemperature( int value ) {
    temperature = (5.0 * value * 100.0) / 1024;

    //TODO refresh the temperature

}
