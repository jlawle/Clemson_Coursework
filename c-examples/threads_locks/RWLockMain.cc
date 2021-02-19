#include "RWObj.h"
#include "sthread.h"

#define NWORKERS 10
static const int NOPS = 10;

void *readSome(void *rwObjPtr)
{
  int ii;
  RWObj *rwObj = (RWObj *)rwObjPtr;
  for(ii = 0; ii < NOPS; ii++){
    rwObj->read();
  }
  return NULL;
}

void *writeSome(void *rwObjPtr)
{
  int ii;
  RWObj *rwObj = (RWObj *)rwObjPtr;
  for(ii = 0; ii < NOPS; ii++){
    rwObj->write(ii);
  }
  return NULL;
}

int main(int argc, char **argv)
{
  int ii;

  RWObj *rwObj = new RWObj();
  sthread_t rworkers[NWORKERS];
  sthread_t wworkers[NWORKERS];

  for(ii = 0; ii < NWORKERS; ii++){
    sthread_create_p(&rworkers[ii], readSome, rwObj);
    sthread_create_p(&wworkers[ii], writeSome, rwObj);
  }

  for(ii = 0; ii < NWORKERS; ii++){
    sthread_join(rworkers[ii]);
    sthread_join(wworkers[ii]);
  }

}
