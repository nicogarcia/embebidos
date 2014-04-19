#ifndef SYSTEMQUEUE_H_
#define SYSTEMQUEUE_H_

typedef void (*fptr)();


// Enqueues functions to do

class SystemQueue {

private:

    int size;
    int head;
    int tail;

    //FIXME: Some other metoth to pass the max ?
    static const int max = 5;
    fptr functions_to_be_called [max];

public:

    SystemQueue();

    //Enqueue a new function to be execute in the SystemQueue
    void enqueue(fptr function );

    //Execute and dequeue the head function in the SystemQueue
    bool execute();

    //Return true if the SystemQueue is empty, false in other case
    bool isEmpty();

};


#endif /* SYSTEMQUEUE_H_ */