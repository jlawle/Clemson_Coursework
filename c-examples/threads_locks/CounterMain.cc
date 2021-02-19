// test code for the Counter shared object

#include "thread.h"
#include "Counter.h"

Counter array1[1000], array2[1000];

void count(Counter *countArray) {
  int i, j;

  for (i = 0; i < 1000; i++)
    for (j = 0; j < 1000; j++)
      countArray[j].increment();
}

void oddcount(Counter *countArray) {
  int i, j;

  for (i = 0; i < 1000; i++)
    for (j = 1; j < 1000; j += 2)
      countArray[j].increment();
}

void evencount(Counter *countArray) {
  int i, j;

  for (i = 0; i < 1000; i++)
    for (j = 0; j < 1000; j += 2)
      countArray[j].increment();
}

void helper() {
  count(array2);
}

void oddhelper() {
  oddcount(array2);
}

main() {
  thread_t thread;

// one thread, one array
  count(array1); 

// two threads, two arrays
  thread_create(&thread, helper, 0)
  count(array1); 
  thread_join(thread);

// two threads, one array
  thread_create(&thread, helper, 0)
  count(array2); 
  thread_join(thread);

// two threads, one array, odd/even
  thread_create(&thread, oddhelper, 0)
  evencount(array2); 
  thread_join(thread);
}
