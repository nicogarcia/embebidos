#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include "StopwatchState.h"

class Stopwatch_ {
public:
    Stopwatch_();
    int last_state_pointer;

    void setCurrentState(int new_state);
    StopwatchState* getCurrentState();
    int getCurrentStatePointer();

    // States
    static StopwatchState states[];
    enum States {
        STATE_MP = 0,
        STATE_MCA = 1,
        STATE_MCD = 2,
        STATE_MVT = 3,
        STATE_MAD = 4,
        STATE_BEFORE_LAUNCH = 5
    };

    // Current time
    static unsigned long current_time;

    // Current bright
    static int current_bright;
private:

    int current_state_pointer;

    // Max time
    static const unsigned long STOPWATCH_MAX_TIME = 5999999;


    // Time when descending counting started
    static unsigned long start_time;
    // Start time of mcd state
    static unsigned long mcd_initial_time;

    // Stored times
    static const int TIMES_LENGTH = 10;
    static long times[TIMES_LENGTH];
    static int times_count;
    static int next_available_index;
    static int viewing_index;

    // State functions
    static void MP_state_action();
    static void MCA_state_action();
    static void MCD_state_action();
    static void MVT_state_action();
    static void MAD_state_action();

    // Transition functions
    static void start_from_last_time();
    static void start_from_zero();
    static void reset_viewing_time();
    static void start_from_max_time();
    static void store_current_time();
    static void show_next_stored_time();
    static void show_previous_stored_time();
    static void increase_lcd_bright();
    static void decrease_lcd_bright();
};
extern Stopwatch_ Stopwatch;

#endif /* STOPWATCH_H_ */