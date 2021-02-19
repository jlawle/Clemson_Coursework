/*
 * SpinLock.h -- simple mutual exclusion lock for x86 
 */
#ifndef _SPINLOCK_H_
#define  _SPINLOCK_H_
#include <pthread.h>

class SpinLock{
  int value;
 public: 
  SpinLock();
  ~SpinLock();
  void acquire();
  void release();
};
#endif
