/*
 * RWEntry.cc -- implementation of shared object that
 *  controls shared access to a database record.
 */
#include <assert.h>
#include <pthread.h>
#include "RWLock.h"

RWLock::RWLock()
{
  activeReaders = 0;
  activeWriters = 0;
  waitingReaders = 0;
  waitingWriters = 0;
}

// Wait until no active or waiting
// writes, then proceed.
void RWLock::startRead() {
    lock.acquire();
    waitingReaders++;
    while (readShouldWait()) {
        readGo.Wait(&lock);
    }
    waitingReaders--;
    activeReaders++;
    lock.release();
}

// Done reading. If no other active
// reads, a write may proceed.
void RWLock::doneRead() {
    lock.acquire();
    activeReaders--;
    if (activeReaders == 0 
         && waitingWriters > 0) {
        writeGo.signal();
    }
    lock.release();
}

// Read waits if any active or waiting
// write ("writers preferred")
bool 
RWLock::readShouldWait() {
    return (activeWriters > 0 
             || waitingWriters > 0);
}


// Wait until no active read or
// write then proceed.
void RWLock::startWrite() {
    lock.acquire();
    waitingWriters++;
    while (writeShouldWait()) {
        writeGo.Wait(&lock);
    }
    waitingWriters--;
    activeWriters++;
    lock.release();
}

// Done writing. A waiting write or
// read may proceed.
void 
RWLock::doneWrite() {
    lock.acquire();
    activeWriters--;
    assert(activeWriters == 0);
    if (waitingWriters > 0) {
        writeGo.signal();
    } 
    else {
        readGo.broadcast();
    }
    lock.release();
}

// Write waits for active read or write 
bool
RWLock::writeShouldWait() {
    return (activeWriters > 0 
             || activeReaders > 0);
}
