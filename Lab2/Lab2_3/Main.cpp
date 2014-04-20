#include "Arduino.h"
#include "LCDKeypadKeys.h"
#include "LCDKeypadDriver.h"
#include "SoftwareSerial.h"
#include "KeyManagement.h"
#include "SystemClock.h"
#include "Stopwatch.h"
#include "Task.h"

void print_time() {
    //Serial.print("Time! ");
    //Serial.print(SystemClock.getMillis());
    //Serial.println("ms");
    PORTB ^= (1 << PB5);
}

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
    // Check system clock events
    SystemClock.checkEvents();
}