#ifndef KEYMANAGEMENT_H_
#define KEYMANAGEMENT_H_

class KeyManagement_ {
public:
    // Register all LCDKeypadDriver events
    KeyManagement_();
private:
    // Long press length in seconds
    static const int LONG_PRESS_LENGHT = 3;

    // Idle time
    unsigned long idle_time;

    // Start time
    unsigned long start_time;

    // Key down single function
    static void key_down_callback();

    // Key up functions
    // Up
    static void up_key_callback();
    // Down
    static void down_key_callback();
    // Select
    static void select_key_callback();

    // Start idle timer
    static void idle_start();

    // Callback for idle time reached
    static void idle_callback();

};
extern KeyManagement_ KeyManagement;


#endif /* KEYMANAGEMENT_H_ */