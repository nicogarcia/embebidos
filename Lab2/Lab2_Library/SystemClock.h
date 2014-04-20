#ifndef SYSTEMCLOCK_H_
#define SYSTEMCLOCK_H_

#include "Task.h"
#include "SystemQueue.h"

// Define C extern for ISR
extern "C" void TIMER2_COMPA_vect(void) __attribute__ ((signal));

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

    // Attach callback to pending queue
    void attach(Task task);

    // Enqueue ready callback to execute
    void enqueue(fptr function);
private:

    // System millis (long)
    volatile unsigned long millis;

    // Initialize TIMER2 parameters
    void init_timer2();

    // Callback variables
    volatile bool event_flag;

    //Array of tasks
    Task tasks[TOTAL_TASKS];

    // A queue of functions to be called
    SystemQueue ReadyTasksQueue;

    // A queue for pending tasks
    SystemQueue PendingTasks;

    // Timer ISR to count millis
    friend void TIMER2_COMPA_vect();

    //Number of task attach
    int tasks_attached;

    Task EmptyTask;
};

extern SystemClock_ SystemClock;

#endif /* SYSTEMCLOCK_H_ */