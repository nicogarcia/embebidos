#ifndef SYSTEMQUEUE_H_
#define SYSTEMQUEUE_H_

typedef void (*fptr)();


// Enqueues functions to do

class SystemQueue {

private:

    int size;
    int head;
    int tail;

    //FIXME: Some other method to pass the max ?
    static const int MAX = 10;
    fptr functions_to_be_called [MAX];

public:

    SystemQueue();

    //Enqueue a new function to be execute in the SystemQueue
    void enqueue(fptr function );

    //Dequeue function
    fptr dequeue();

    //Execute and dequeue the head function in the SystemQueue
    bool execute();

    //Return true if the SystemQueue is empty, false in other case
    bool isEmpty();

};


#endif /* SYSTEMQUEUE_H_ */