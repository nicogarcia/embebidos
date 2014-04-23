#ifndef STOPWATCHSTATE_H_
#define STOPWATCHSTATE_H_

class StopwatchState {
public:
    StopwatchState(void (*)());

    void setEventResponse(int key_event, int next_state, void (*Action)());

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

    void RunStateAction() {
        state_action();
    }
private:
    class Response {
    public:
        Response() {
            nextState = -1;
        }
        int nextState;
        void (*Action)();
    };

    void (*state_action)();
    static const int CALLBACK_TYPE_COUNT = 7;

    // Store next state and callbacks for each state, simple and long keypress
    Response responses[CALLBACK_TYPE_COUNT];
};


#endif /* STOPWATCHSTATE_H_ */