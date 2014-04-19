#ifndef SYSTEMCLOCK_H_
#define SYSTEMCLOCK_H_

#include "Task.h"
#include "SystemQueue.h"

// Define C extern for ISR
extern "C" void TIMER0_COMPA_vect(void) __attribute__ ((signal));

//
typedef void (*fptr)();

// Takes account of the system modulo 2**32
// 2**32 millis are 50 days
class SystemClock_ {
public:
    SystemClock_();
    // Get system millis
    unsigned long getMillis();

    // Checks if there's a callback to execute
    void checkEvents();

    unsigned long getTaskStartTime(int i);

    unsigned long getTaskTime(int i);

    //Return the pointer of the task callback function
    fptr getTaskCallback(int i);

    void deleteTask(int i);

    static const int TOTAL_TASKS = 5;

private:

    // System millis (long)
    unsigned long millis;

    // Initialize TIMER2 parameters
    void init();

    // Callback variables
    unsigned long time_ms;
    bool event_flag;

    //Array of tasks
    Task tasks[TOTAL_TASKS];

    // A queue of functions to be called
    SystemQueue functions_to_be_called;

    // Attach callback
    void attach(Task task);

    // Timer ISR to count millis
    friend void TIMER0_COMPA_vect();

    //Number of task attach
    int task_attached;

    Task EmptyTask;
};

extern SystemClock_ SystemClock;

#endif /* SYSTEMCLOCK_H_ */