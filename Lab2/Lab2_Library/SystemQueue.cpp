#include "SystemQueue.h"
#include "Arduino.h"


//Implementation of a Array Queue to store and execute functions


SystemQueue::SystemQueue() {
    size = 0;
    head = 0;
    tail = max - 1;
    for (int i = 0; i < max; i++)
        functions_to_be_called[i] = NULL;
}

//Enqueue a function in the tail of the queue
void SystemQueue::enqueue(fptr function ) {
    if (size <= max) {
        tail = (tail + 1) % max;
        functions_to_be_called[tail] = function;
        size++;
    }
}

//Execute and dequeue a function
bool SystemQueue::execute() {
    if (size > 0) {
        functions_to_be_called[head]();
        head = (head + 1) % max;
        size--;
        return true;
    }
    return false;
}


bool SystemQueue::isEmpty() {
    return size == 0;
}
