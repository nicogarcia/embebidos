#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include "StopwatchState.h"

class Stopwatch_ {
public:
    Stopwatch_();
    StopwatchState* current_state;

private:
    // Max time
    static const unsigned long MAX_TIME = 594158;

    // States
    static StopwatchState MCA, MCD, MP, MVT, MAD;

    // Current time
    static unsigned long current_time;

    // Stored times
    static const int TIMES_LENGTH = 10;
    static long times[TIMES_LENGTH];
    static int times_count;
    static int next_available_index;
    static int viewing_index;

    // Current bright
    static int current_bright;

    // Registered in the driver to handle all key down events
    // Initiates a timer to detect long press
    void key_down_handler();

    // Registered in the driver to handle all key up events
    // Checks the press timer to determine if it was a long press
    void key_up_handler();

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