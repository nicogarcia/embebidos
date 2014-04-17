#include "Arduino.h"
#include "LCDKeypadKeys.h"
#include "LCDKeypadDriver.h"
#include "SoftwareSerial.h"
#include "SystemClock.h"

void setup() {
    //Inicializar el pin del led como salida
    DDRB |= (1<<DDB5);

    Serial.begin(115200);
    Serial.print("Lab2_3 Start!!\n");

}

void loop() {
    // Check system clock events
    SystemClock.checkEvents();

    // Check and execute if there were events triggered
    KeypadDriver.checkEvents();
}