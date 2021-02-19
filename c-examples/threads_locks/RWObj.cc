#include <assert.h>
#include "RWObj.h"
#include <stdio.h>
#include "sthread.h"

RWObj::RWObj()
{
  val1 = 0;
  val2 = 0;
}

void
RWObj::read()
{
  int val;
  rwLock.startRead();
  val = val1;
  sthread_yield();
  printf("Read: %d %d\n", val1, val2);
  assert(val1 == val2);
  assert(val == val1);
  rwLock.doneRead();
}

void
RWObj::write(int val)
{
  rwLock.startWrite();
  val1 = val;
  val2 = val;
  sthread_yield();
  printf("write: %d %d\n", val1, val2);
  assert(val1 == val2);
  assert(val == val1);
  rwLock.doneWrite();
}
