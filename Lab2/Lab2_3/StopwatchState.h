#ifndef STOPWATCHSTATE_H_
#define STOPWATCHSTATE_H_

class StopwatchState {
public:
    StopwatchState(void (*)());

    void setEventResponse(int key_event, StopwatchState* next_state, void (*Action)());

    // Executes its Response to the key_event
    void execute(int key_event);

    enum KEY_EVENT {
        SELECT = 0,
        UP = 1,
        DOWN = 2,
        SELECT_LONG = 3,
        UP_LONG = 4,
        DOWN_LONG = 5,
        NONE = 6
    };
private:
    class Response {
    public:
        Response() {
            nextState = NULL;
        }
        Response(StopwatchState* ns, void (*action)()): nextState(ns), Action(action) {}
        StopwatchState* nextState;
        void (*Action)();
    };

    void (*state_action)();
    static const int CALLBACK_TYPE_COUNT = 3;

    // Store next state and callbacks for each state, simple and long keypress
    Response responses[CALLBACK_TYPE_COUNT * 2];

};


#endif /* STOPWATCHSTATE_H_ */