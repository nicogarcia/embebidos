#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include "StopwatchState.h"

class Stopwatch {
	public:

	private:
	StopwatchState MCA, MCD, MP, MVT, MAD;
	StopwatchState current_state;
	
	// Registers the key handlers and initializes states
	// and its associated Responses
	void init();

	// Registered in the driver to handle all key down events
	// Initiates a timer to detect long press
	void key_down_handler();

	// Registered in the driver to handle all key up events
	// Checks the press timer to determine if it was a long press
	void key_up_handler();
	
};

#endif /* STOPWATCH_H_ */