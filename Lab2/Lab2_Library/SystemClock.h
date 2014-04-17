#ifndef SYSTEMCLOCK_H_
#define SYSTEMCLOCK_H_

// Define C extern for ISR
extern "C" void TIMER0_COMPA_vect(void) __attribute__ ((signal));

// Takes account of the system modulo 2**32
// 2**32 millis are 50 days
class SystemClock_ {
public:
    SystemClock_();
    // Get system millis
    unsigned long getMillis();

    // Checks if there's a callback to execute
    void checkEvents();
private:
    // System millis (long)
    unsigned long millis;

    // Initialize TIMER2 parameters
    void init();

    // Callback variables
    unsigned long time_ms;
    bool event_flag;

    // Start time for callback
    unsigned long start_time;

    // Tick callback
    void (*callback)();

    // Function to be called
    void (*function_to_be_called)();

    // Attach callback
    void attach(void (*callback)(), long time_ms);

    // Timer ISR to count millis
    friend void TIMER0_COMPA_vect();
};
extern SystemClock_ SystemClock;

#endif /* SYSTEMCLOCK_H_ */