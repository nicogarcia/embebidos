#include "LCDKeypadKeys.h"

// Threshold values for AD conversion
unsigned int LCDKeypadKeys::ADCKeyValues[5] = { 30, 150, 360, 535, 760 };
	
// Key names for text I/O
const char* LCDKeypadKeys::KEY_NAMES[] = { "RIGHT", "UP", "DOWN", "LEFT", "SELECT"};

// Converts an analog input to digital using threshold values
int LCDKeypadKeys::GetKey(unsigned int analog_input){
	int k;
	for (k = 0; k < KEY_COUNT; k++)
		if (analog_input < ADCKeyValues[k])
			return k;

	if (k >= KEY_COUNT)
    	k = -1;     // No valid key pressed

    return k;
}