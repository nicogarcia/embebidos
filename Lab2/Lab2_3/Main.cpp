#include "Arduino.h"
#include "LCDKeypadKeys.h"
#include "LCDKeypadDriver.h"
#include "SoftwareSerial.h"
#include "KeyManagement.h"
#include "SystemClock.h"
#include "Stopwatch.h"
#include "Task.h"
#include "StopwatchUI.h"

void setup() {
    ui.initScreen();

    //Inicializar el pin del led como salida
    DDRB |= (1<<DDB5);

    Serial.begin(115200);

    //  Update current mode screen
    ui.updateUI();
}

void loop() {
    Stopwatch.getCurrentState()->RunStateAction();

    // Check system clock events
    SystemClock.checkEvents();
}