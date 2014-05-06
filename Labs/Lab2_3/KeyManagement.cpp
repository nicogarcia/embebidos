#include "Arduino.h"
#include "KeyManagement.h"
#include "LCDKeypadDriver.h"
#include "LCDKeypadKeys.h"
#include "Stopwatch.h"
#include "StopwatchState.h"
#include "SystemClock.h"
#include "StopwatchUI.h"

KeyManagement_ KeyManagement;

unsigned long KeyManagement_::long_enabled_time;
unsigned long KeyManagement_::key_down_time;

// Stores key states (down or up), and long press
bool KeyManagement_::key_states[4];

void KeyManagement_::reset_key_states() {
    for (int i = 0; i < 4; i++)
        key_states[i] = false;
}

KeyManagement_::KeyManagement_() {
    idle = false;
    key_down_time = 0;
    long_enabled_time = false;

    KeypadDriver.registerOnKeyDownCallback(UP_key_down_callback, LCDKeypadKeys::KEY_UP);
    KeypadDriver.registerOnKeyDownCallback(DOWN_key_down_callback, LCDKeypadKeys::KEY_DOWN);
    KeypadDriver.registerOnKeyDownCallback(SELECT_key_down_callback, LCDKeypadKeys::KEY_SELECT);

    KeypadDriver.registerOnKeyUpCallback(UP_key_callback, LCDKeypadKeys::KEY_UP);
    KeypadDriver.registerOnKeyUpCallback(DOWN_key_callback, LCDKeypadKeys::KEY_DOWN);
    KeypadDriver.registerOnKeyUpCallback(SELECT_key_callback, LCDKeypadKeys::KEY_SELECT);
}

void KeyManagement_::key_down_common() {
    // Store start time to check press length
    KeyManagement.key_down_time = SystemClock.getMillis();

    // Disable UI message if present
    ui.disable_message_print();

    SystemClock.attach(Task((unsigned long)LONG_PRESS_LENGHT_MS, long_key_down_callback));
    long_enabled_time = SystemClock.getMillis();

    //  Update keys
    ui.updateUI();

    // Disable inactivity flag
    KeyManagement.idle = false;
}
// Key down functions
// Up
void KeyManagement_::UP_key_down_callback() {
    // Reset states and set Up key as pressed
    reset_key_states();
    key_states[KEY_STATE_UP] = true;

    // Do common work for key down
    key_down_common();

    // Debug
    Serial.print("UP Key down | ");
}

void KeyManagement_::DOWN_key_down_callback() {
    // Reset states and set Up key as pressed
    reset_key_states();
    key_states[KEY_STATE_DOWN] = true;

    // Do common work for key down
    key_down_common();

    // Debug
    Serial.print("UP Key down | ");
}

void KeyManagement_::SELECT_key_down_callback() {
    // Reset states and set Up key as pressed
    reset_key_states();
    key_states[KEY_STATE_SELECT] = true;

    // Do common work for key down
    key_down_common();

    // Debug
    Serial.print("UP Key down | ");
}

void KeyManagement_::long_key_down_callback() {
    unsigned long time_since_key_down = SystemClock.getMillis() - KeyManagement.key_down_time;

    // Determine if key is UP or UP_LONG
    if(time_since_key_down < LONG_PRESS_LENGHT_MS)
        return;

    key_states[KEY_STATE_LONG] = true;

    //  Update long key press indicator
    ui.updateUI();
}

// Key up functions
// Up
void KeyManagement_::UP_key_callback() {
    int key;
    unsigned long time_since_key_down = SystemClock.getMillis() - KeyManagement.key_down_time;

    // Determine if key is UP or UP_LONG
    if((time_since_key_down >= LONG_PRESS_LENGHT_MS) && key_states[KEY_STATE_LONG]) {
        key = StopwatchState::UP_LONG;
        Serial.println("UP_LONG up");
    } else {
        key = StopwatchState::UP;
        Serial.println("UP up");
    }

    // Set all keys as not pressed
    reset_key_states();

    Stopwatch.getCurrentState()->execute(key);
    KeyManagement.key_down_time = SystemClock.getMillis();

    //  Update current mode screen
    ui.updateUI();

    // Start idle timer
    idle_start();
}

// Down
void KeyManagement_::DOWN_key_callback() {
    int key;
    unsigned long time_since_key_down = SystemClock.getMillis() - KeyManagement.key_down_time;

    // Determine if key is DOWN or DOWN_LONG
    //if(time_since_key_down < LONG_PRESS_LENGHT_MS) {
    if((time_since_key_down >= LONG_PRESS_LENGHT_MS) && key_states[KEY_STATE_LONG]) {
        key = StopwatchState::DOWN_LONG;
        Serial.println("DOWN_LONG up");
    } else {
        key = StopwatchState::DOWN;
        Serial.println("DOWN up");
    }

    // Set all keys as not pressed
    reset_key_states();

    Stopwatch.getCurrentState()->execute(key);
    KeyManagement.key_down_time = SystemClock.getMillis();

    //  Update current mode screen
    ui.updateUI();

    // Start idle timer
    idle_start();
}

// Select
void KeyManagement_::SELECT_key_callback() {
    int key;
    unsigned long time_since_key_down = SystemClock.getMillis() - KeyManagement.key_down_time;

    // Determine if key is SELECT or SELECT_LONG
    //if(time_since_key_down < LONG_PRESS_LENGHT_MS) {

    if((time_since_key_down >= LONG_PRESS_LENGHT_MS) && key_states[KEY_STATE_LONG]) {
        key = StopwatchState::SELECT_LONG;
        Serial.println("SELECT_LONG up");
    } else {
        key = StopwatchState::SELECT;
        Serial.println("SELECT up");
    }

    // Set all keys as not pressed
    reset_key_states();

    Stopwatch.getCurrentState()->execute(key);
    KeyManagement.key_down_time = SystemClock.getMillis();

    //  Update current mode screen
    ui.updateUI();

    // Start idle timer
    idle_start();
}

void KeyManagement_::idle_start() {
    // Set idle state as true, if there's a keydown this flag will be unset
    KeyManagement.idle = true;

    // Store the latest time that idle period started to check its lenght
    KeyManagement.idle_start_time = SystemClock.getMillis();

    // Schedule task to be launched after IDLE_TIME_MS passed to run idle routines
    SystemClock.attach(Task(IDLE_TIME_MS, idle_callback));
}

// Idle function callback
void KeyManagement_::idle_callback() {
    // Get idle time period
    unsigned long idle_time = SystemClock.getMillis() - KeyManagement.key_down_time;

    // If system is still idle and time is longer than idle lenght, then take idle-state actions
    if(KeyManagement.idle && (idle_time >= (long) IDLE_TIME_MS) &&
            (Stopwatch.getCurrentStatePointer() == Stopwatch.STATE_MAD)) {

        Stopwatch.getCurrentState()->execute(StopwatchState::NONE);
        //  Update current mode screen
        ui.updateUI();
    }
}