#ifndef _BBQ_H_
#define _BBQ_H_
#include "Lock.h"
#include "CV.h"
#include "thread.h"

// BBQ.h 
// Thread-safe blocking queue.

const int MAX = 10;

class BBQ{
  // Synchronization variables
    Lock lock;
    CV itemAdded;
    CV itemRemoved;

  // State variables
    int items[MAX];
    int front;
    int nextEmpty;

  public: 
    BBQ();
    ~BBQ() {};
    void insert(int item);
    int remove();
};
#endif
