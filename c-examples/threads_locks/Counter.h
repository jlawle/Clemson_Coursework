// Thread-safe counter

#include "SpinLock.h"

class Counter {
  private:
    SpinLock spinlock;
    int value;
  public:
    Counter();
    ~Counter();
    void increment();
}
