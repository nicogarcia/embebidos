#include "Arduino.h"
#include "Stopwatch.h"
#include "StopwatchState.h"

StopwatchState Stopwatch_::MAD = StopwatchState(&MAD_state_action);
StopwatchState Stopwatch_::MCA = StopwatchState(Stopwatch_::MCA_state_action);
StopwatchState Stopwatch_::MCD = StopwatchState(Stopwatch_::MCD_state_action);
StopwatchState Stopwatch_::MVT = StopwatchState(Stopwatch_::MVT_state_action);
StopwatchState Stopwatch_::MP = StopwatchState(Stopwatch_::MP_state_action);

Stopwatch_::Stopwatch_() {

    // MP State config
    MP.setEventResponse(StopwatchState::UP,
                        new Response(&MCA, NULL));

    MP.setEventResponse(StopwatchState::DOWN,
                        new Response(&MCD, NULL));

    MP.setEventResponse(StopwatchState::SELECT,
                        new Response(&MVT, NULL));

    MP.setEventResponse(StopwatchState::SELECT_LONG,
                        new Response(&MAD, NULL));

    // MCA State config
    MCA.setEventResponse(StopwatchState::UP,
                         new Response(&MCA, store_current_time));

    MCA.setEventResponse(StopwatchState::SELECT,
                         new Response(&MP, NULL));

    // MCD State config
    MCD.setEventResponse(StopwatchState::DOWN,
                         new Response(&MCD, store_current_time));

    MCD.setEventResponse(StopwatchState::SELECT,
                         new Response(&MP, NULL));

    // MVT State config
    MVT.setEventResponse(StopwatchState::UP,
                         new Response(&MVT, show_next_stored_time));

    MVT.setEventResponse(StopwatchState::DOWN,
                         new Response(&MVT, show_previous_stored_time));

    MVT.setEventResponse(StopwatchState::SELECT,
                         new Response(&MP, NULL));

    // MAD State config
    MAD.setEventResponse(StopwatchState::UP,
                         new Response(&MAD, increase_lcd_bright));

    MAD.setEventResponse(StopwatchState::DOWN,
                         new Response(&MAD, decrease_lcd_bright));

    MAD.setEventResponse(StopwatchState::SELECT,
                         new Response(&MP, NULL));

}

void Stopwatch_::MP_state_action() {
    // Pause Stopwatch
    // Show current time
}

void Stopwatch_::MCA_state_action() {
    // Get current Stopwatch time
    // Show current time
}

void Stopwatch_::MCD_state_action() {
    // Get current Stopwatch time
    // Show current time
}

void Stopwatch_::MVT_state_action() {
    // Show 'current_viewing_time'
}

void Stopwatch_::MAD_state_action() {
    // Show current time
    // Show current bright
}

void Stopwatch_::store_current_time() {
    //  Save current time
    // Show message ? How ?
}

void Stopwatch_::show_next_stored_time() {
    // Increase current viewing time pointer
}

void Stopwatch_::show_previous_stored_time() {
    // Decrease current viewing time pointer
}

void Stopwatch_::increase_lcd_bright() {
    // Increase lcd bright
}

void Stopwatch_::decrease_lcd_bright() {
    // Decrease lcd bright
}