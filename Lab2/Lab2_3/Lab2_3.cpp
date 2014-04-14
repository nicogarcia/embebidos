#include "Arduino.h"
#include "LCDKeypadKeys.h"
#include "LCDKeypadDriver.h"
#include "SoftwareSerial.h"

void setup(){
	//Inicializar el pin del led como salida
	DDRB |= (1<<DDB5);
	
	Serial.begin(9600);
	Serial.print("Lab2_3 Start!!\n");
	
}

void loop(){
	// Check and execute if there were events triggered
	LCDKeypadDriver::Instance()->checkEvents();
}