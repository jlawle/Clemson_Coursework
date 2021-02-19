#include "SpinLock.h"

SpinLock::SpinLock() {
  value = 0;
}

SpinLock::~SpinLock() {}

void
SpinLock::acquire() {
  while(test_and_set(&value)) 
    ; // spin
}

void
SpinLock::release() {
  atomic_clear(&value);
}
