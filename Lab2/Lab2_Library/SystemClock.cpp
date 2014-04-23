#include "Arduino.h"
#include "SystemClock.h"
#include <avr/io.h>
#include <avr/interrupt.h>

SystemClock_ SystemClock;

SystemClock_::SystemClock_() {

    tasks_attached = 0;

    milliseconds = 0;
    event_flag = false;

    EmptyTask = Task();

    ReadyTasksQueue = SystemQueue();

    for(int i = 0; i < TOTAL_TASKS; i++)
        tasks[i] = EmptyTask;

    init_timer2();
}

// Get system millis
unsigned long SystemClock_::getMillis() {
    return millis();
}

// Initialize TIMER0 parameters
void SystemClock_::init_timer2() {
    // Initialize timer0
    noInterrupts();           // Disable all interrupts

    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2  = 0;				  // Initialize counter

    OCR2A = 249;              // Compare match register 16MHz/64=250KHz --> 1ms*250KHz -1 = 249
    TCCR2A |= (1 << WGM21) ;   // CTC mode

    TCCR2B |= (1 << CS22);    // 64 prescaler

    TIMSK2 |= (1 << OCIE2A);  // Enable timer compare interrupt
    interrupts();             // Enable all interrupts
}

// Attach callback
void SystemClock_::attach(Task task) {
    if (tasks_attached < TOTAL_TASKS) {
        //put the task in the array of to do task
        for (int i = 0; i < TOTAL_TASKS; i++) {
            if (tasks[i].callback == NULL) {
                tasks[i] = task;

                //set the start_time (the current time)
                tasks[i].start_time = getMillis();
                break;
            }
        }
        tasks_attached++;
    }
}

// Check if there's a callback to call
//TODO: if  attach is executing when there are functions to be called,
void SystemClock_::checkEvents() {
    while (ReadyTasksQueue.execute()) {}
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
        tasks_attached--;
    }
}

void SystemClock_::enqueue( fptr function ) {
    ReadyTasksQueue.enqueue(function);
}

// Timer ISR to count millis
void TIMER2_COMPA_vect() {
    // Unnecesary, arduino provides millis()
    //SystemClock.milliseconds++;

    for (int i = 0; i < SystemClock.TOTAL_TASKS; i++) {
        // FIXME: Redo this conditional
        if(SystemClock.tasks[i].callback != NULL) {
            //When it's time to execute the function
            if(SystemClock.getMillis() - SystemClock.getTaskStartTime(i) >= SystemClock.getTaskTime(i)) {
                //Enqueue the function to do.
                SystemClock.ReadyTasksQueue.enqueue(SystemClock.getTaskCallback(i));
                //delete the task
                SystemClock.deleteTask(i);
            }
        }
    }
}