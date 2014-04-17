#ifndef STOPWATCHSTATE_H_
#define STOPWATCHSTATE_H_

class Response;

class StopwatchState {
public:
    StopwatchState(void (*)());
    void setEventResponse(int key_event, Response* response);

    // Executes its Response to the key_event
    void execute(int key_event);

    enum KEY_EVENT {
        SELECT = 0,
        UP = 1,
        DOWN = 2,
        SELECT_LONG = 3,
        UP_LONG = 4,
        DOWN_LONG = 5
    };
private:
    void (*state_action)();
    static const int CALLBACK_TYPE_COUNT = 3;

    // Store next state and callbacks for each state, simple and long keypress
    Response* responses[CALLBACK_TYPE_COUNT * 2];

};

class Response {
public:
    Response(StopwatchState* ss, void (*action)()) : nextState(ss), Action(action) { }

    StopwatchState* nextState;
    void (*Action)();
};

#endif /* STOPWATCHSTATE_H_ */