#include "Arduino.h"
#include "Stopwatch.h"
#include "StopwatchState.h"
#include "StopwatchUI.h"
#include "SystemClock.h"

// Current time
unsigned long Stopwatch_::current_time = 0;

// Time when counting started
unsigned long Stopwatch_::start_time;
// Initial time of mca state
unsigned long Stopwatch_::mca_initial_time = 0;
// Initial time of mcd state
unsigned long Stopwatch_::mcd_initial_time = Stopwatch_::STOPWATCH_MAX_TIME;

// Stored times
long Stopwatch_::times[TIMES_LENGTH];
int Stopwatch_::times_count = 0;
int Stopwatch_::next_available_index = 0;
int Stopwatch_::viewing_index = 0;

// Current bright
int Stopwatch_::current_bright = 80;

StopwatchState Stopwatch_::states[] = {
    StopwatchState(MP_state_action),
    StopwatchState(MCA_state_action),
    StopwatchState(MCD_state_action),
    StopwatchState(MVT_state_action),
    StopwatchState(MAD_state_action),
    StopwatchState(INIT_state_action)
};

// Global Stopwatch "instance"
Stopwatch_ Stopwatch = Stopwatch_();

Stopwatch_::Stopwatch_() {
    current_state_pointer = STATE_INIT;

    // MP State config
    states[STATE_MP].setEventResponse(StopwatchState::UP, STATE_MCA, start_from_last_time);

    states[STATE_MP].setEventResponse(StopwatchState::UP_LONG, STATE_MCA, start_from_zero);

    states[STATE_MP].setEventResponse(StopwatchState::DOWN, STATE_MCD, start_from_last_time);

    states[STATE_MP].setEventResponse(StopwatchState::DOWN_LONG, STATE_MCD, start_from_max_time);

    states[STATE_MP].setEventResponse(StopwatchState::SELECT_LONG, STATE_MVT, reset_viewing_time);

    states[STATE_MP].setEventResponse(StopwatchState::SELECT, STATE_MAD, NULL);

    // MCA State config
    states[STATE_MCA].setEventResponse(StopwatchState::UP, STATE_MCA, store_current_time);

    states[STATE_MCA].setEventResponse(StopwatchState::SELECT, STATE_MP, NULL);

    // MCD State config
    states[STATE_MCD].setEventResponse(StopwatchState::DOWN, STATE_MCD, store_current_time);

    states[STATE_MCD].setEventResponse(StopwatchState::SELECT, STATE_MP, NULL);

    // MVT State config
    states[STATE_MVT].setEventResponse(StopwatchState::UP, STATE_MVT, show_next_stored_time);

    states[STATE_MVT].setEventResponse(StopwatchState::DOWN, STATE_MVT, show_previous_stored_time);

    states[STATE_MVT].setEventResponse(StopwatchState::SELECT, STATE_MP, NULL);

    // MAD State config
    states[STATE_MAD].setEventResponse(StopwatchState::UP, STATE_MAD, increase_lcd_bright);

    states[STATE_MAD].setEventResponse(StopwatchState::DOWN, STATE_MAD, decrease_lcd_bright);

    states[STATE_MAD].setEventResponse(StopwatchState::SELECT, STATE_MP, NULL);

    // MAD Idle time transition
    states[STATE_MAD].setEventResponse(StopwatchState::NONE, STATE_MP, NULL);

    //INIT state config
    states[STATE_INIT].setEventResponse(StopwatchState::UP, STATE_MP, NULL);

    states[STATE_INIT].setEventResponse(StopwatchState::UP_LONG, STATE_MP, NULL);

    states[STATE_INIT].setEventResponse(StopwatchState::DOWN, STATE_MP, NULL);

    states[STATE_INIT].setEventResponse(StopwatchState::DOWN_LONG, STATE_MP, NULL);

    states[STATE_INIT].setEventResponse(StopwatchState::SELECT, STATE_MP, NULL);

    states[STATE_INIT].setEventResponse(StopwatchState::SELECT_LONG, STATE_MP, NULL);
}

// Stopwatch stops refreshing
void Stopwatch_::MP_state_action() {
}

void Stopwatch_::MCA_state_action() {
    // Get current Stopwatch time
    current_time = (mca_initial_time + SystemClock.getMillis() - start_time) % STOPWATCH_MAX_TIME;
    //  Update current mode screen
    ui.updateUI();
}

void Stopwatch_::MCD_state_action() {
    // Get current Stopwatch time
    current_time = (mcd_initial_time - (SystemClock.getMillis() - start_time));
    // Turn over to max (when there's overflow)
    if (current_time > STOPWATCH_MAX_TIME) {
        start_time = SystemClock.getMillis();
        mcd_initial_time = STOPWATCH_MAX_TIME;
    }
    //  Update current mode screen
    ui.updateUI();
}

void Stopwatch_::MVT_state_action() {
}

void Stopwatch_::MAD_state_action() {
}

void Stopwatch_::INIT_state_action() {
}

// Load last time as current time
void Stopwatch_::start_from_last_time() {
    // Set start time to calculate time differences
    start_time = SystemClock.getMillis();

    // If there are stored times, load the latest, else start from default
    if(times_count == 0) {
        mca_initial_time = 0;
        mcd_initial_time = STOPWATCH_MAX_TIME;

        ui.show_no_saved_message();
        ui.updateUI();
    } else {
        mcd_initial_time = times[(next_available_index - 1) % TIMES_LENGTH];
        mca_initial_time = mcd_initial_time;
    }
}

void Stopwatch_::start_from_zero() {
    start_time = SystemClock.getMillis();
    mca_initial_time = 0;
    current_time = 0;
}

void Stopwatch_::start_from_max_time() {
    start_time = SystemClock.getMillis();
    mcd_initial_time = STOPWATCH_MAX_TIME;
    current_time = 0;
}

void Stopwatch_::reset_viewing_time() {
    viewing_index = (next_available_index - 1) % TIMES_LENGTH;
}

void Stopwatch_::store_current_time() {
    //  Save current time
    times[next_available_index] = current_time;

    // Increase writing pointer
    next_available_index = (next_available_index + 1) % TIMES_LENGTH;

    // Increase times count if not full
    times_count = min(times_count + 1, TIMES_LENGTH);

    ui.show_saved_time_message(current_time);
    ui.updateUI();
}

// Increase current viewing time pointer
void Stopwatch_::show_next_stored_time() {
    // Calculate the first saved time
    int first = (next_available_index - times_count) % TIMES_LENGTH;

    // Calculate the following viewing time
    int next_viewing_pointer = (viewing_index + 1) % times_count;

    // If the following viewing index is the first or the latest saved time, ignore command
    if((next_viewing_pointer == first) || (next_viewing_pointer == next_available_index))
        return;

    // Increase viewing pointer
    viewing_index = next_viewing_pointer;
}

// Decrease current viewing time pointer
void Stopwatch_::show_previous_stored_time() {
    // Calculate the first saved time
    int first = (next_available_index - times_count) % TIMES_LENGTH;

    // If viewing is next_avialable, ignore the command, else decrease pointer
    if(viewing_index == first)
        return;

    // Decrement viewing pointer
    viewing_index = (viewing_index - 1) % times_count;
}

void Stopwatch_::increase_lcd_bright() {
    // Increase lcd bright
    current_bright = min(100, current_bright + 20);
    // Set lcd bright
    analogWrite(10, current_bright);
}

void Stopwatch_::decrease_lcd_bright() {
    // Decrease lcd bright
    current_bright = max(0, current_bright - 20);
    // Set lcd bright
    analogWrite(10, current_bright);
}

void Stopwatch_::setCurrentState( int new_state) {
    current_state_pointer = new_state;
}

StopwatchState* Stopwatch_::getCurrentState() {
    return &states[current_state_pointer];
}

int Stopwatch_::getCurrentStatePointer() {
    return current_state_pointer;
}