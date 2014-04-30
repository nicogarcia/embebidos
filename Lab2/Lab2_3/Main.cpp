#include "Arduino.h"
#include "LCDKeypadKeys.h"
#include "LCDKeypadDriver.h"
#include "SoftwareSerial.h"
#include "KeyManagement.h"
#include "SystemClock.h"
#include "Stopwatch.h"
#include "Task.h"
#include "StopwatchUI.h"
#include "ADCManager.h"

void setup() {
    KeypadDriver.initialize();

    ADCManager.insertDriver(KeypadDriver, 0);

    ui.initScreen();

    //Inicializar el pin del led como salida
    DDRB |= (1 << DDB5);

    Serial.begin(115200);

    //  Update current mode screen
    ui.updateUI();
}

void loop() {
    Stopwatch.getCurrentState()->RunStateAction();

    // Check system clock events
    SystemClock.checkEvents();
}