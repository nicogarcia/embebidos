#include "Arduino.h"
#include "LCDKeypadKeys.h"
#include "LCDKeypadDriver.h"
#include "SoftwareSerial.h"
#include "KeyManagement.h"
#include "SystemClock.h"
#include "Stopwatch.h"
#include "Task.h"
#include "UserInterface.h"

// these constants won't change.  But you can change the size of
// your LCD using them:
const int numRows = 2;
const int numCols = 16;

void init_lcd() {
    pinMode(10, OUTPUT);
    // set up the LCD's number of columns and rows:
    UserInterface.lcd.begin(numCols,numRows);

    analogWrite(10, 100); //Controla intensidad backlight
}

void setup() {
    init_lcd();

    //Inicializar el pin del led como salida
    DDRB |= (1<<DDB5);

    Serial.begin(115200);
    Serial.print("Lab2_3 Start!!\n");

    /*
        SystemClock.attach(Task(500, print_time));
        SystemClock.attach(Task(1000, print_time));*/
}

void loop() {
    Stopwatch.getCurrentState()->RunStateAction();

    // Check system clock events
    SystemClock.checkEvents();
}