#include "Arduino.h"
#include "LCDKeypadKeys.h"
#include "LCDKeypadDriver.h"
#include "SoftwareSerial.h"
#include <util/delay.h>

/*int val = 0;
int analogPin = 0;
int last_key1 = 0;*/

void printUpKeyDown() {
    Serial.print("UP: Key Down!\n");
}
void printUpKeyUp() {
    Serial.print("UP: Key Up!\n");
}
void printDownKeyDown() {
    Serial.print("DOWN: Key Down!\n");
}
void printDownKeyUp() {
    Serial.print("DOWN: Key Up!\n");
}
void printLeftKeyDown() {
    Serial.print("LEFT: Key Down!\n");
}
void printLeftKeyUp() {
    Serial.print("LEFT: Key Up!\n");
}
void printRightKeyDown() {
    Serial.print("RIGHT: Key Down!\n");
}
void printRightKeyUp() {
    Serial.print("RIGHT: Key Up!\n");
}
void printSelectKeyDown() {
    Serial.print("SELECT: Key Down!\n");
}
void printSelectKeyUp() {
    Serial.print("SELECT: Key Up!\n");
}

void setup() {
    //Inicializar el pin del led como salida
    DDRB |= (1<<DDB5);

    Serial.begin(115200);
    Serial.print("Lab_2_2 Start!! Waiting for Analog inputs...\n");

    KeypadDriver.registerOnKeyDownCallback(printUpKeyDown, LCDKeypadKeys::KEY_UP);
    KeypadDriver.registerOnKeyUpCallback(printUpKeyUp, LCDKeypadKeys::KEY_UP);

    KeypadDriver.registerOnKeyDownCallback(printDownKeyDown, LCDKeypadKeys::KEY_DOWN);
    KeypadDriver.registerOnKeyUpCallback(printDownKeyUp, LCDKeypadKeys::KEY_DOWN);

    KeypadDriver.registerOnKeyDownCallback(printLeftKeyDown, LCDKeypadKeys::KEY_LEFT);
    KeypadDriver.registerOnKeyUpCallback(printLeftKeyUp, LCDKeypadKeys::KEY_LEFT);

    KeypadDriver.registerOnKeyDownCallback(printRightKeyDown, LCDKeypadKeys::KEY_RIGHT);
    KeypadDriver.registerOnKeyUpCallback(printRightKeyUp, LCDKeypadKeys::KEY_RIGHT);

    KeypadDriver.registerOnKeyDownCallback(printSelectKeyDown, LCDKeypadKeys::KEY_SELECT);
    KeypadDriver.registerOnKeyUpCallback(printSelectKeyUp, LCDKeypadKeys::KEY_SELECT);

}

void loop() {
    /* 	Polling example to test the emulated keyboard

    val = LCDKeypadKeys::GetKey(analogRead(analogPin));
    if(val != last_key1){
    	if(val == -1)
    		Serial.println("NONE");
    	else
    		Serial.println(LCDKeypadKeys::KEY_NAMES[val]);
    }

    last_key1 = val;
    _delay_ms(100);*/

    // Check and execute if there were events triggered
    KeypadDriver.checkEvents();
}