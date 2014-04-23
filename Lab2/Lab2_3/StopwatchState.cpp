#include "Arduino.h"
#include "Stopwatch.h"
#include "StopwatchState.h"
#include "LCDUI.h"

StopwatchState::StopwatchState(void (*state_action)()) {
    this->state_action = state_action;
}

void StopwatchState::setEventResponse(int key_event, int next_state, void (*Action)() ) {
    responses[key_event].nextState = next_state;
    responses[key_event].Action = Action;
}

// Executes its Response to the key_event
void StopwatchState::execute(int key_event) {
    // FIXME: Not optimal way to check that responses[key_event] is not initialized
    if(responses[key_event].nextState != -1) {
        // If there's an action attached to this state
        if(responses[key_event].Action != NULL)
            responses[key_event].Action(); // Execute action

        Stopwatch.setCurrentState(responses[key_event].nextState);
    }
}