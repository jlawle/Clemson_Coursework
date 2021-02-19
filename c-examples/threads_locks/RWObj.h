#ifndef _RWOBJ_H_
#define _RWOBJ_H_
#include "RWLock.h"

class RWObj{
 private:
  // Synchronization variables
  RWLock rwLock;

  // State variables
  int val1;
  int val2;
  
 public:
  RWObj();
  ~RWObj() {};
  void read();
  void write(int v);
};
#endif
