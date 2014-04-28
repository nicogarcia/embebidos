#include "Arduino.h"
#include "TempMonitor.h"
#include "LCDKeypadDriver.h"
#include "TempUI.h"

TempMonitor_ TempMonitor;

const char* TempMonitor_::state_names[STATES_COUNT] = {
    "  TEMP ACTUAL",
    "  TEMP MAXIMA",
    "  TEMP MINIMA",
    " TEMP PROMEDIO"
};

TempMonitor_::TempMonitor_() {
    current_state = STATE_CURRENT_TEMP;

    data[STATE_CURRENT_TEMP] = 0;
    data[STATE_MAX_TEMP] = 0;
    data[STATE_MIN_TEMP] = 0;
    data[STATE_AVG_TEMP] = 0;

    history_next_available = 0;

    history_count = 0;

    KeypadDriver.registerOnKeyUpCallback(advance_state, LCDKeypadKeys::KEY_SELECT);
}

void TempMonitor_::newTemperatureSensed() {
    // TODO: Read read temperature
    int temperature = 0;

    // Update current temperature
    data[STATE_CURRENT_TEMP] = temperature;

    // Update max and average accum
    long avg_accum = 0;
    for(int i = 0; i < history_count; i++) {
        data[STATE_MAX_TEMP] = max(data[STATE_MAX_TEMP], temp_history[i]);
        data[STATE_MIN_TEMP] = min(data[STATE_MIN_TEMP], temp_history[i]);
        avg_accum += temp_history[i];
    }

    // Update average
    data[STATE_AVG_TEMP] = avg_accum / history_count;

    // Update UI
    ui.updateUI();
}

void TempMonitor_::advance_state() {
    // Advance state circularly
    TempMonitor.current_state = (TempMonitor.current_state + 1) % TempMonitor.STATES_COUNT;

    // Update UI to see the new state
    ui.updateUI();
}
