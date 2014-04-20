#include "Arduino.h"
#include "Stopwatch.h"
#include "StopwatchState.h"
#include "UserInterface.h"
#include "SystemClock.h"

Stopwatch_::Stopwatch_() {
    MAD = StopwatchState(MAD_state_action);
    MCA = StopwatchState(MCA_state_action);
    MCD = StopwatchState(MCD_state_action);
    MVT = StopwatchState(MVT_state_action);
    MP = StopwatchState(MP_state_action);

    next_available_index = 0;
    times_count = 0;
    viewing_index = 0;

    current_state = &MP;

    // MP State config
    MP.setEventResponse(StopwatchState::UP, &MCA, (Stopwatch_::*) &start_from_last_time);

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
    // TODO: Show current time
    Serial.print("PAUSE MODE: ");
    UserInterface.printTime(SystemClock.getMillis() / 10);
}

void Stopwatch_::MCA_state_action() {
    // TODO: Get current Stopwatch time
    // TODO: Show current time
    Serial.print("MCA MODE: ");
    UserInterface.printTime(SystemClock.getMillis() / 10);
}

void Stopwatch_::MCD_state_action() {
    // TODO: Get current Stopwatch time
    // TODO: Show current time
    Serial.print("MCD MODE: ");
    UserInterface.printTime(SystemClock.getMillis() / 10);
}

void Stopwatch_::MVT_state_action() {
    // Show 'current_viewing_time'
    if(Stopwatch_::times_count == 0) {
        // TODO: Display NO SAVED TIMES message
        Serial.println("MVT: No saved times");
    } else {
        // TODO: Display viewing index
        UserInterface.printTime(times[viewing_index]);
    }
}

void Stopwatch_::MAD_state_action() {
    // TODO: Show current time
    // TODO: Show current bright
}

// Load last time as current time
void Stopwatch_::start_from_last_time() {
    if(times_count == 0) {
        // TODO: Display NO SAVED TIMES message
        Serial.println("Cannot continue from last saved time. No saved times.");
    } else
        current_time = times[(next_available_index - 1) % times_count];
}

void Stopwatch_::start_from_zero() {
    current_time = 0;
}

void Stopwatch_::start_from_max_time() {
    current_time = 594158;
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
    times_count = min(times_count, TIMES_LENGTH);

    // TODO: Show message ? How ?
    Serial.println("Time saved OK!");
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
}

void Stopwatch_::decrease_lcd_bright() {
    current_bright = max(0, current_bright - 20);
    // TODO: Decrease lcd bright
}