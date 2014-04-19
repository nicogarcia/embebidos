#include "Arduino.h"
#include "SystemClock.h"
#include <avr/io.h>
#include <avr/interrupt.h>

SystemClock_ SystemClock;


SystemClock_::SystemClock_() {

    task_attached = 0;

    millis = 0;
    event_flag = false;

    EmptyTask = Task();

    functions_to_be_called = SystemQueue();
}

// Get system millis
unsigned long SystemClock_::getMillis() {
    return millis;
}

// Initialize TIMER0 parameters
void SystemClock_::init() {
    // Initialize timer0
    noInterrupts();           // Disable all interrupts

    TCCR2A = 0;				  // TCCR1A isn't necessary
    TCCR2B = 0;
    TCNT2  = 0;				  // Initialize counter

    OCR2A = 249;              // Compare match register 16MHz/64=250KHz --> 1ms*250KHz -1 = 249
    TCCR2B |= (1 << WGM21);   // CTC mode

    TCCR2B |= (1 << CS21);    // We need to count 1ms so...  | CS22 | CS21 | CS20 |
    TCCR2B |= (1 << CS20);	  // 64 prescaler			    |	0	 | 1	    |	1     |

    TIMSK2 |= (1 << OCIE2A);  // Enable timer compare interrupt
    interrupts();             // Enable all interrupts
}

// Attach callback
void SystemClock_::attach(Task task) {
    if (task_attached < TOTAL_TASKS) {
        //put the task in the array of to do task
        for (int i = 0; i < TOTAL_TASKS; i++) {
            if (tasks[i].callback == NULL) {
                tasks[i] = task;

                //set the start_time (the current time)
                tasks[i].start_time = getMillis();
                break;
            }
        }
        task_attached++;
    }
}

// Check if there's a callback to call
//TODO: if  attach is executing when there are functions to be call,
void SystemClock_::checkEvents() {
    if (event_flag) {
        while (functions_to_be_called.execute()) {}
        event_flag = false;
    }

}

unsigned long SystemClock_::getTaskTime(int i) {
    if (i < TOTAL_TASKS)
        return tasks[i].time;
    return 0;
}

unsigned long SystemClock_::getTaskStartTime(int i) {
    if (i < TOTAL_TASKS)
        return tasks[i].start_time;
    return 0;
}

fptr SystemClock_::getTaskCallback(int i) {
    if (i < TOTAL_TASKS)
        return tasks[i].callback;
    return 0;
}

void SystemClock_::deleteTask(int i) {
    if (i < TOTAL_TASKS) {
        tasks[i] = EmptyTask;
        task_attached--;
    }
}

// Timer ISR to count millis
void TIMER0_COMPA_vect() {
    SystemClock.millis++;
    for (int i = 0; i < SystemClock.TOTAL_TASKS; i++) {
        //When it's time to execute the function
        if(SystemClock.millis - SystemClock.getTaskStartTime(i) >= SystemClock.getTaskTime(i)) {
            //Enqueue the function to do.
            SystemClock.functions_to_be_called.enqueue(SystemClock.getTaskCallback(i));
            //delete the task
            SystemClock.deleteTask(i);
        }
        if (!SystemClock.functions_to_be_called.isEmpty())
            SystemClock.event_flag = true;
    }
}