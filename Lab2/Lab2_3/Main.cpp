#include "Arduino.h"
#include "LCDKeypadKeys.h"
#include "LCDKeypadDriver.h"
#include "SoftwareSerial.h"
#include "KeyManagement.h"
#include "SystemClock.h"
#include "Stopwatch.h"
#include "Task.h"

void setup() {
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