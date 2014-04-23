#ifndef KEYMANAGEMENT_H_
#define KEYMANAGEMENT_H_

class KeyManagement_ {
public:
    // Register all LCDKeypadDriver events
    KeyManagement_();

    enum KeyStates {
        KEY_STATE_UP = 0,
        KEY_STATE_DOWN = 1,
        KEY_STATE_SELECT = 2,
        KEY_STATE_LONG = 3
    };
    // Stores key states (down or up), and long press
    static bool key_states[4];

    // Idle state flag
    bool idle;
    unsigned long idle_start_time;

    // Start time
    static unsigned long key_down_time;

    // Long enabled
    static unsigned long long_enabled_time;

private:
    // Long press length in seconds
    static const int LONG_PRESS_LENGHT_MS = 2000;

    // Idle time
    static const int IDLE_TIME_MS = 5000;

    // Common key down work
    static void key_down_common();

    // Key down functions
    static void UP_key_down_callback();
    static void DOWN_key_down_callback();
    static void SELECT_key_down_callback();

    // Key up functions
    static void UP_key_callback();
    static void DOWN_key_callback();
    static void SELECT_key_callback();

    // Long keypress time reached callback
    static void long_key_down_callback();

    // Start idle timer
    static void idle_start();

    // Callback for idle time reached
    static void idle_callback();

    // Reset key states to false (not pressed)
    static void reset_key_states();

};
extern KeyManagement_ KeyManagement;


#endif /* KEYMANAGEMENT_H_ */