#include "Arduino.h"
#include "Stopwatch.h"
#include "StopwatchState.h"
#include "UserInterface.h"
#include "SystemClock.h"

// Current time
unsigned long Stopwatch_::current_time = 0;

// Time when counting started
unsigned long Stopwatch_::start_time;
// Start time of mcd state
unsigned long Stopwatch_::mcd_initial_time;

// Stored times
long Stopwatch_::times[TIMES_LENGTH];
int Stopwatch_::times_count = 0;
int Stopwatch_::next_available_index = 0;
int Stopwatch_::viewing_index = 0;

// Current bright
int Stopwatch_::current_bright = 60;

StopwatchState Stopwatch_::MAD = StopwatchState(MAD_state_action);
StopwatchState Stopwatch_::MCA = StopwatchState(MCA_state_action);
StopwatchState Stopwatch_::MCD = StopwatchState(MCD_state_action);
StopwatchState Stopwatch_::MVT = StopwatchState(MVT_state_action);
StopwatchState Stopwatch_::MP = StopwatchState(MP_state_action);

// Global Stopwatch "instance"
Stopwatch_ Stopwatch = Stopwatch_();

Stopwatch_::Stopwatch_() {
    current_state = &MP;
    last_state = NULL;

    // MP State config
    MP.setEventResponse(StopwatchState::UP, &MCA, start_from_last_time);

    MP.setEventResponse(StopwatchState::UP_LONG, &MCA, start_from_zero);

    MP.setEventResponse(StopwatchState::DOWN, &MCD, start_from_last_time);

    MP.setEventResponse(StopwatchState::DOWN_LONG, &MCD, start_from_max_time);

    MP.setEventResponse(StopwatchState::SELECT, &MVT, reset_viewing_time);

    MP.setEventResponse(StopwatchState::SELECT_LONG, &MAD, NULL);

    // MCA State config
    MCA.setEventResponse(StopwatchState::UP, &MCA, store_current_time);

    MCA.setEventResponse(StopwatchState::SELECT, &MP, NULL);

    // MCD State config
    MCD.setEventResponse(StopwatchState::DOWN, &MCD, store_current_time);

    MCD.setEventResponse(StopwatchState::SELECT, &MP, NULL);

    // MVT State config
    MVT.setEventResponse(StopwatchState::UP, &MVT, show_next_stored_time);

    MVT.setEventResponse(StopwatchState::DOWN, &MVT, show_previous_stored_time);

    MVT.setEventResponse(StopwatchState::SELECT, &MP, NULL);

    // MAD State config
    MAD.setEventResponse(StopwatchState::UP, &MAD, increase_lcd_bright);

    MAD.setEventResponse(StopwatchState::DOWN, &MAD, decrease_lcd_bright);

    MAD.setEventResponse(StopwatchState::SELECT, &MP, NULL);

    // MAD Idle time transition
    MAD.setEventResponse(StopwatchState::NONE, &MP, NULL);
}

// Stopwatch stops refreshing
void Stopwatch_::MP_state_action() {
    //  Show current time
    UserInterface.printModeAndTime(MODE_PAUSE, current_time);
}

void Stopwatch_::MCA_state_action() {
    // Get current Stopwatch time
    current_time = SystemClock.getMillis() - start_time;
    //  Show current time
    UserInterface.printModeAndTime(MODE_MCA, current_time);
}

void Stopwatch_::MCD_state_action() {
    // Get current Stopwatch time
    current_time = mcd_initial_time - (SystemClock.getMillis() - start_time);
    // Show current time
    UserInterface.printModeAndTime(MODE_MCD, current_time);
}

void Stopwatch_::MVT_state_action() {
    // Show 'current_viewing_time'
    if(times_count == 0) {
        // TODO: Display NO SAVED TIMES message
        UserInterface.printNoSavedTimes();
    } else {
        // TODO: Display viewing index
        UserInterface.printModeAndTime(MODE_MVT, times[viewing_index]);
    }
}

void Stopwatch_::MAD_state_action() {
    // TODO: Show current time?
    // Show current bright
    UserInterface.printBright(current_bright);
}

// Load last time as current time
void Stopwatch_::start_from_last_time() {
    if(times_count == 0) {
        // TODO: Display NO SAVED TIMES message
        Serial.println("Cannot continue from last saved time. No saved times.");
    } else {
        start_time = SystemClock.getMillis();
        current_time = times[(next_available_index - 1) % times_count];
    }
}

void Stopwatch_::start_from_zero() {
    Serial.println("Starting from zero...");
    start_time = SystemClock.getMillis();
    current_time = 0;
}

void Stopwatch_::start_from_max_time() {
    Serial.println("Starting from max time...");
    start_time = SystemClock.getMillis();
    // FIXME: Get this max out!
    mcd_initial_time = 5999999;
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

    // TODO: Show message ? How ?
    Serial.print("Time saved OK! => ");
    UserInterface.printTime(current_time);
}

// Increase current viewing time pointer
void Stopwatch_::show_next_stored_time() {
    // If viewing time is the latest stored, ignore action
    if((viewing_index + 1) % times_count == next_available_index)
        return;

    viewing_index = (viewing_index + 1) % times_count;
}

// Decrease current viewing time pointer
void Stopwatch_::show_previous_stored_time() {
    // If the array is full, then the last to show should be 'next_available_index'
    if(times_count == TIMES_LENGTH) {
        // If viewing is next_avialable, ignore the command, else decrease pointer
        if(viewing_index == next_available_index)
            return;
        else
            viewing_index = (viewing_index - 1) % times_count;
    } else
        viewing_index = max(0, viewing_index - 1);
}

void Stopwatch_::increase_lcd_bright() {
    current_bright = min(100, current_bright + 20);
    // TODO: Increase lcd bright
    analogWrite(10, current_bright);
}

void Stopwatch_::decrease_lcd_bright() {
    current_bright = max(0, current_bright - 20);
    // TODO: Decrease lcd bright
    analogWrite(10, current_bright);
}

void Stopwatch_::setCurrentState( StopwatchState* new_state ) {
    last_state = current_state;
    current_state = new_state;
}

StopwatchState* Stopwatch_::getCurrentState() {
    return current_state;
}
