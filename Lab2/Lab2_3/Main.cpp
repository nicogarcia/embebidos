#include "Arduino.h"
#include "LCDKeypadKeys.h"
#include "LCDKeypadDriver.h"
#include "SoftwareSerial.h"
#include "KeyManagement.h"
#include "SystemClock.h"
#include "Stopwatch.h"
#include "Task.h"
#include "LCDUI.h"

void setup() {
    LCDUI::initScreen();

    //Inicializar el pin del led como salida
    DDRB |= (1<<DDB5);

    Serial.begin(115200);
    Serial.print("Lab2_3 Start!!\n");

    //  Update current mode screen
    lcd_ui.updateUI();
}

void loop() {
    Stopwatch.getCurrentState()->RunStateAction();

    // Check system clock events
    SystemClock.checkEvents();
}