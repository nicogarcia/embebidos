#include "Arduino.h"
#include "SoftwareSerial.h"
#include "SystemClock.h"
#include "LCDKeypadKeys.h"
#include "LCDKeypadDriver.h"
#include "TempUI.h"
#include "TempSensorDriver.h"
#include "TempMonitor.h"
#include "ADCManager.h"

void setup() {
    lm35.initialize();
    KeypadDriver.initialize();

    ADCManager.insertDriver(lm35, 1);
    ADCManager.insertDriver(KeypadDriver, 0);

    // Initialize lcd screen
    ui.initScreen();

    //Inicializar el pin del led como salida
    DDRB |= (1<<DDB5);

    Serial.begin(115200);
    Serial.print("Lab3_1 Start!!\n");

    ui.updateUI();
}

void loop() {
    // Check and execute if there were events triggered
    SystemClock.checkEvents();
}