#ifndef TEMPMONITOR_H_
#define TEMPMONITOR_H_

class TempMonitor_ {
public:
    TempMonitor_();

    // Current state
    int current_state;

    // States count
    static const int STATES_COUNT = 4;

    // States names to print in UI
    static const char *state_names[4];

    // Temperature data
    double data[STATES_COUNT];

    // Data pointer
    int history_next_available;

    // Data accessors
    enum STATE_POINTERS {
        STATE_CURRENT_TEMP = 0,
        STATE_MAX_TEMP = 1,
        STATE_MIN_TEMP = 2,
        STATE_AVG_TEMP = 3
    };

    // Temperature history
    static const int HISTORY_LENGHT = 100;
    int history_count;
    double temp_history[HISTORY_LENGHT];
    // Method to call when there's a new temperature available
    static void newTemperatureSensed();

    static void advance_state();
    static void regress_state();

    static void TempSensorCallback();

    // Key states
    bool up_key;
    bool down_key;
    static void up_key_down();
    static void down_key_down();
};
extern TempMonitor_ TempMonitor;

#endif /* TEMPMONITOR_H_ */