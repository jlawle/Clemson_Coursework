/*
 * RWEntry.h -- interface to shared object that controls
 * shared access to a database record.
 */
#ifndef _RWEntry_H_
#define _RWEntry_H_
#include "Lock.h"
#include "CV.h"

class RWLock{
  private:
    // Synchronization variables
    Lock lock;
    CV readGo;
    CV writeGo;

    // State variables
    int activeReaders;
    int activeWriters;
    int waitingReaders;
    int waitingWriters;

  public:
    RWLock();
    ~RWLock() {};
    void startRead();
    void doneRead();
    void startWrite();
    void doneWrite();

  private:
    bool readShouldWait();
    bool writeShouldWait();
};
#endif
