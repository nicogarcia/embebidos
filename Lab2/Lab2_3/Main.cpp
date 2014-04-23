#include "Arduino.h"
#include "LCDKeypadKeys.h"
#include "LCDKeypadDriver.h"
#include "SoftwareSerial.h"
#include "KeyManagement.h"
#include "SystemClock.h"
#include "Stopwatch.h"
#include "Task.h"
#include "LCDUI.h"

// TODO: Should this be here?
// these constants won't change.  But you can change the size of
// your LCD using them:
const int numRows = 2;
const int numCols = 16;

void init_lcd() {
    pinMode(10, OUTPUT);
    // set up the LCD's number of columns and rows:
    LCDUI::screen.begin(numCols,numRows);

    analogWrite(10, 80); //Controla intensidad backlight
}

void setup() {
    init_lcd();

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