#include "Arduino.h"
#include "TempMonitor.h"
#include "LCDKeypadDriver.h"
#include "TempSensorDriver.h"
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
    data[STATE_MIN_TEMP] = 500;
    data[STATE_AVG_TEMP] = 0;

    history_next_available = 0;

    history_count = 0;

    KeypadDriver.registerOnKeyUpCallback(advance_state, LCDKeypadKeys::KEY_SELECT);
}

void TempMonitor_::newTemperatureSensed() {
    // TODO: Read read temperature
    double temperature = lm35.temperature;

    // Update current temperature
    TempMonitor.data[STATE_CURRENT_TEMP] = temperature;

    // TODO: AVG COUNT UPDATE
    TempMonitor.history_count = max(TempMonitor.history_count + 1, TempMonitor.HISTORY_LENGHT);
    TempMonitor.temp_history[TempMonitor.history_next_available] = temperature;
    TempMonitor.history_next_available = (TempMonitor.history_next_available + 1) % TempMonitor.HISTORY_LENGHT;

    // Update max and average accum
    double avg_accum = 0;
    for(int i = 0; i < TempMonitor.history_count; i++) {
        TempMonitor.data[STATE_MAX_TEMP] = max(TempMonitor.data[STATE_MAX_TEMP], TempMonitor.temp_history[i]);
        TempMonitor.data[STATE_MIN_TEMP] = min(TempMonitor.data[STATE_MIN_TEMP], TempMonitor.temp_history[i]);
        avg_accum += TempMonitor.temp_history[i];
    }

    // Update average
    TempMonitor.data[STATE_AVG_TEMP] = avg_accum / (double) TempMonitor.history_count;

    // Update UI
    ui.updateUI();
}

void TempMonitor_::advance_state() {
    // Advance state circularly
    TempMonitor.current_state = (TempMonitor.current_state + 1) % TempMonitor.STATES_COUNT;

    // Update UI to see the new state
    ui.updateUI();
}

void TempMonitor_::TempSensorCallback() {
    ui.updateUI();
}
