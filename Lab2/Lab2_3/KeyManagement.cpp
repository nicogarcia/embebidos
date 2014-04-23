#include "Arduino.h"
#include "KeyManagement.h"
#include "LCDKeypadDriver.h"
#include "LCDKeypadKeys.h"
#include "Stopwatch.h"
#include "StopwatchState.h"
#include "SystemClock.h"
#include "LCDUI.h"

KeyManagement_ KeyManagement;

// Stores key states (down or up), and long press
bool KeyManagement_::key_states[4];

void KeyManagement_::reset_key_states() {
    for (int i = 0; i < 4; i++)
        key_states[i] = false;
}

KeyManagement_::KeyManagement_() {
    idle = false;
    key_down_time = 0;

    KeypadDriver.registerOnKeyDownCallback(UP_key_down_callback, LCDKeypadKeys::KEY_UP);
    KeypadDriver.registerOnKeyDownCallback(DOWN_key_down_callback, LCDKeypadKeys::KEY_DOWN);
    KeypadDriver.registerOnKeyDownCallback(SELECT_key_down_callback, LCDKeypadKeys::KEY_SELECT);

    KeypadDriver.registerOnKeyUpCallback(UP_key_callback, LCDKeypadKeys::KEY_UP);
    KeypadDriver.registerOnKeyUpCallback(DOWN_key_callback, LCDKeypadKeys::KEY_DOWN);
    KeypadDriver.registerOnKeyUpCallback(SELECT_key_callback, LCDKeypadKeys::KEY_SELECT);
}

void key_down_common() {
    // Store start time to check press length
    KeyManagement.key_down_time = SystemClock.getMillis();

    //  Update keys
    lcd_ui.updateUI();

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
    key_states[KEY_STATE_LONG] = true;

    //  Update long key press indicator
    lcd_ui.updateUI();
}

// Key up functions
// Up
void KeyManagement_::UP_key_callback() {
    int key;
    int time_since_key_down = (SystemClock.getMillis() - KeyManagement.key_down_time) / 1000;

    // Set all keys as not pressed
    reset_key_states();

    // Determine if key is UP or UP_LONG
    if(time_since_key_down < LONG_PRESS_LENGHT) {
        key = StopwatchState::UP;
        Serial.println("UP up");
    } else {
        key = StopwatchState::UP_LONG;
        Serial.println("UP_LONG up");
    }

    Stopwatch.getCurrentState()->execute(key);

    //  Update current mode screen
    lcd_ui.updateUI();

    // Start idle timer
    idle_start();
}

// Down
void KeyManagement_::DOWN_key_callback() {
    int key;
    int time_since_key_down = (SystemClock.getMillis() - KeyManagement.key_down_time) / 1000;

    // Set all keys as not pressed
    reset_key_states();

    // Determine if key is DOWN or DOWN_LONG
    if(time_since_key_down < LONG_PRESS_LENGHT) {
        key = StopwatchState::DOWN;
        Serial.println("DOWN up");
    } else {
        key = StopwatchState::DOWN_LONG;
        Serial.println("DOWN_LONG up");
    }

    Stopwatch.getCurrentState()->execute(key);

    //  Update current mode screen
    lcd_ui.updateUI();

    // Start idle timer
    idle_start();
}

// Select
void KeyManagement_::SELECT_key_callback() {
    int key;
    int time_since_key_down = (SystemClock.getMillis() - KeyManagement.key_down_time) / 1000;

    // Set all keys as not pressed
    reset_key_states();

    // Determine if key is SELECT or SELECT_LONG
    if(time_since_key_down < LONG_PRESS_LENGHT) {
        key = StopwatchState::SELECT;
        Serial.println("SELECT up");
    } else {
        key = StopwatchState::SELECT_LONG;
        Serial.println("SELECT_LONG up");
    }

    Stopwatch.getCurrentState()->execute(key);

    //  Update current mode screen
    lcd_ui.updateUI();

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
        lcd_ui.updateUI();
    }
}
