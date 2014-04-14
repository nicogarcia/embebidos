#ifndef STOPWATCHSTATE_H_
#define STOPWATCHSTATE_H_
#include <pair>

typedef pair<StopwatchState*, void (*)()> Response;

class StopwatchState {
	public:
	void setEventResponse(int key_event, Response* response);

	// Executes its Response to the key_event
	void execute(int key_event);
	
	private:
	static int CALLBACK_TYPE_COUNT = 3;
	enum KEY_EVENT {
		SELECT = 0,
		UP = 1,
		DOWN = 2,
		SELECT_LONG = 3,
		UP_LONG = 4,
		DOWN_LONG = 5
	};
	
	// Store next state and callbacks for each state, simple and long keypress
	Response* responses[CALLBACK_TYPE_COUNT * 2];
	
};

#endif /* STOPWATCHSTATE_H_ */