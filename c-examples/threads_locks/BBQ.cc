#include <assert.h>
#include <pthread.h>
#include "BBQ.h"

// BBQ.cc 
// thread-safe blocking queue

// Wait until there is room and
// then insert an item.
void
BBQ::insert(int item) {
    lock.acquire();
  
    while ((nextEmpty - front) == MAX) {
        itemRemoved.wait(&lock);
    }
    items[nextEmpty % MAX] = item;
    nextEmpty++;

    itemAdded.signal();
    lock.release();
}

// Wait until there is an item and 
// then remove an item.
int
BBQ::remove() {
    int item;

    lock.acquire();

    while (front == nextEmpty) {
        itemAdded.wait(&lock);
    }
    item = items[front % MAX];
    front++;

    itemRemoved.signal();
    lock.release();
    return item;
}

// Initialize the queue to empty,
// the lock to free, and the
// condition variables to empty.
BBQ::BBQ() {
    front = nextEmpty = 0;
}
