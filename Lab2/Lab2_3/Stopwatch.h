#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include "StopwatchState.h"

class Stopwatch_ {
public:
    Stopwatch_();
    StopwatchState* current_state;

private:
    static StopwatchState MCA, MCD, MP, MVT, MAD;

    // Registers the key handlers and initializes states
    // and its associated Responses
    void init();

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
    static void store_current_time();

    static void show_next_stored_time();

    static void show_previous_stored_time();

    static void increase_lcd_bright();

    static void decrease_lcd_bright();
};

extern Stopwatch_ Stopwatch;

#endif /* STOPWATCH_H_ */