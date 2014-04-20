#include "Arduino.h"
#include "UserInterface.h"
#include "SoftwareSerial.h"

UserInterface_ UserInterface;

void UserInterface_::printTime(unsigned long time) {
    int minutes = 0, seconds = 0, hundreths = 0;
    hundreths = time % 100;
    seconds = (time / 100) % 60;
    minutes = (time / 6000);

    Serial.print(minutes);
    Serial.print(":");
    Serial.print(seconds);
    Serial.print(".");
    Serial.println(hundreths);
}

