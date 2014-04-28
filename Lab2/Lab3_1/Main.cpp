#include "Arduino.h"
#include "SoftwareSerial.h"
#include "SystemClock.h"
#include "LCDKeypadKeys.h"
#include "LCDKeypadDriver.h"
#include "TempUI.h"

void setup() {
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