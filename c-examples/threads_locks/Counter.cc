#include "Counter.h"

void Counter::Counter() {
  value = 0;
}
void Counter::~Counter() {}

void Counter::increment() {
  spinlock.Acquire();
  value++;
  spinlock.Release();
}

