#ifndef LCDKEYPADKEYS_H
#define LCDKEYPADKEYS_H

// Describes LCDKeypad keys and converts
// an analog signal to a key
class LCDKeypadKeys {
private:
	static unsigned int ADCKeyValues[5];
public:
	enum Keys {
		KEY_RIGHT = 0,
		KEY_UP = 1,
		KEY_DOWN = 2,
		KEY_LEFT = 3,
		KEY_SELECT = 4
	};
	
	static const char* KEY_NAMES[5];

	static const int KEY_COUNT = 5;

	static int GetKey(unsigned int analog_input);
};

#endif
