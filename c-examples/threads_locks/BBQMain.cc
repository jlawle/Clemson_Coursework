#include <assert.h>
#include <stdio.h>
#include "thread.h"
#include "BBQ.h"


void *putSome(void *tsqueuePtr)
{
  int ii;
  BBQ *queue = (BBQ *)tsqueuePtr;

  for(ii = 0; ii < 100; ii++){
    queue->insert(ii);
  }
  return NULL;
}  


int main(int argc, char **argv)
{
  BBQ *queues[3];
  thread_t workers[3];
  int ii, jj, ret;

  // Start the worker threads
  for(ii = 0; ii < 3; ii++){
    queues[ii] = new BBQ();
    thread_create_p(&workers[ii], putSome, 
                     queues[ii]);
  }

  // Remove from the queues
  for(ii = 0; ii < 3; ii++){
    printf("Queue %d:\n", ii);
    for(jj = 0; jj < 20; jj++){
      ret = queues[ii]->remove();
      printf("Got %d\n", ret);
    }
  }
}
