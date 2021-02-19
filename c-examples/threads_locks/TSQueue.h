#ifndef _TSQUEUE_H_
#define _TSQUEUE_H_
#include "Lock.h"
#include "thread.h"

// TSQueue.h 
// Thread-safe queue interface

const int MAX = 10;

class TSQueue {
  // Synchronization variables
    Lock lock;

  // State variables
    int items[MAX];
    int front;
    int nextEmpty;

  public: 
    TSQueue();
    ~TSQueue(){};
    bool tryInsert(int item);
    bool tryRemove(int *item);
};
#endif
