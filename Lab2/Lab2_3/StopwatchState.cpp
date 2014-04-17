#include "Arduino.h"
#include "Stopwatch.h"
#include "StopwatchState.h"

StopwatchState::StopwatchState(void (*state_action)()) {
    this->state_action = state_action;
    for (int i = 0; i < CALLBACK_TYPE_COUNT; i++)
        responses[i] = NULL;
}

void StopwatchState::setEventResponse(int key_event, Response* response) {
    responses[key_event] = response;
}

// Executes its Response to the key_event
void StopwatchState::execute(int key_event) {
    if(responses[key_event] != NULL) {
        if((responses[key_event]->Action) != NULL)
            (responses[key_event]->Action)();
        Stopwatch.current_state = responses[key_event]->nextState;
    }
}