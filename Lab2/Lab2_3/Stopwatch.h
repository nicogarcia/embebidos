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
    StopwatchState MCA, MCD, MP, MVT, MAD;

    // Current time
    unsigned long current_time;

    // Stored times
    static const int TIMES_LENGTH = 10;
    long times[TIMES_LENGTH];
    int times_count;
    int next_available_index;
    int viewing_index;

    // Current bright
    int current_bright;

    // State functions
    void MP_state_action();
    void MCA_state_action();
    void MCD_state_action();
    void MVT_state_action();
    void MAD_state_action();

    // Transition functions
    void start_from_last_time();

    void start_from_zero();

    void reset_viewing_time();

    void start_from_max_time();

    void store_current_time();

    void show_next_stored_time();

    void show_previous_stored_time();

    void increase_lcd_bright();

    void decrease_lcd_bright();
};

extern Stopwatch_ Stopwatch;

#endif /* STOPWATCH_H_ */