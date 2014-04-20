#include "SystemQueue.h"
#include "Arduino.h"


//Implementation of a Array Queue to store and execute functions


SystemQueue::SystemQueue() {
    size = 0;
    head = 0;
    tail = MAX - 1;
    for (int i = 0; i < MAX; i++)
        functions_to_be_called[i] = NULL;
}

//Enqueue a function in the tail of the queue
void SystemQueue::enqueue(fptr function) {
    if (size <= MAX) {
        tail = (tail + 1) % MAX;
        functions_to_be_called[tail] = function;
        size++;
    }
}

fptr SystemQueue::dequeue() {
    if (size > 0) {
        fptr head_element = functions_to_be_called[head];
        head = (head + 1) % MAX;
        size--;
        return head_element;
    }
    return NULL;
}

//Execute and dequeue a function
bool SystemQueue::execute() {
    fptr to_exec = dequeue();
    if (to_exec != NULL) {
        to_exec();
        return true;
    }
    return false;
}

bool SystemQueue::isEmpty() {
    return size == 0;
}
