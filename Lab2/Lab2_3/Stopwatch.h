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

    // Maximum times stored
    static const int TIMES_LENGTH = 10;

    // Total count of times recorded
    static int times_count;

    // Times recorded
    static long times[TIMES_LENGTH];

    // Current viewing recorded time
    static int viewing_index;
private:
    int current_state_pointer;

    // Max time ¡in ms!
    static const unsigned long STOPWATCH_MAX_TIME = 5999999;

    // Time when counting started
    static unsigned long start_time;
    // Initial time of mca state
    static unsigned long mca_initial_time;
    // Initial time of mcd state
    static unsigned long mcd_initial_time;

    static int next_available_index;

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