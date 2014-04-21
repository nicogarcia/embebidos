#include "Arduino.h"
#include "KeyManagement.h"
#include "LCDKeypadDriver.h"
#include "LCDKeypadKeys.h"
#include "Stopwatch.h"
#include "StopwatchState.h"
#include "SystemClock.h"

KeyManagement_ KeyManagement;

KeyManagement_::KeyManagement_() {
    idle = false;
    start_time = 0;

    const int key_count = 3;
    const int keys[] = { LCDKeypadKeys::KEY_UP,
                         LCDKeypadKeys::KEY_DOWN, LCDKeypadKeys::KEY_SELECT
                       };

    for(int i = 0; i < key_count; i++)
        KeypadDriver.registerOnKeyDownCallback(key_down_callback, keys[i]);

    KeypadDriver.registerOnKeyUpCallback(up_key_callback, LCDKeypadKeys::KEY_UP);
    KeypadDriver.registerOnKeyUpCallback(down_key_callback, LCDKeypadKeys::KEY_DOWN);
    KeypadDriver.registerOnKeyUpCallback(select_key_callback, LCDKeypadKeys::KEY_SELECT);
}

// Key down single function
void KeyManagement_::key_down_callback() {
    KeyManagement.start_time = SystemClock.getMillis();
    Serial.print("Key down | ");
    KeyManagement.idle = false;
}

// Key up functions
// Up
void KeyManagement_::up_key_callback() {
    int key;
    int time_since_key_down = (SystemClock.getMillis() - KeyManagement.start_time) / 1000;

    // Determine if key is UP or UP_LONG
    if(time_since_key_down < LONG_PRESS_LENGHT) {
        key = StopwatchState::UP;
        Serial.println("UP up");
    } else {
        key = StopwatchState::UP_LONG;
        Serial.println("UP_LONG up");
    }

    Stopwatch.getCurrentState()->execute(key);

    // Start idle timer
    idle_start();
}

// Down
void KeyManagement_::down_key_callback() {
    int key;
    int time_since_key_down = (SystemClock.getMillis() - KeyManagement.start_time) / 1000;

    // Determine if key is DOWN or DOWN_LONG
    if(time_since_key_down < LONG_PRESS_LENGHT) {
        key = StopwatchState::DOWN;
        Serial.println("DOWN up");
    } else {
        key = StopwatchState::DOWN_LONG;
        Serial.println("DOWN_LONG up");
    }

    Stopwatch.getCurrentState()->execute(key);

    // Start idle timer
    idle_start();
}

// Select
void KeyManagement_::select_key_callback() {
    int key;
    int time_since_key_down = (SystemClock.getMillis() - KeyManagement.start_time) / 1000;

    // Determine if key is SELECT or SELECT_LONG
    if(time_since_key_down < LONG_PRESS_LENGHT) {
        key = StopwatchState::SELECT;
        Serial.println("SELECT up");
    } else {
        key = StopwatchState::SELECT_LONG;
        Serial.println("SELECT_LONG up");
    }

    Stopwatch.getCurrentState()->execute(key);

    // Start idle timer
    idle_start();
}

void KeyManagement_::idle_start() {
    // Set idle state as true, if there's a keydown this flag will be unset
    KeyManagement.idle = true;
    // Schedule task to be launched after IDLE_TIME_MS passed to run idle routines
    SystemClock.attach(Task(IDLE_TIME_MS, idle_callback));
}

// Idle function callback
void KeyManagement_::idle_callback() {
    // If system is still idle (no keydown event triggered)
    if(KeyManagement.idle)
        Stopwatch.getCurrentState()->execute(StopwatchState::NONE);
}
