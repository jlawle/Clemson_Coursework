/*
  * bzero.c -- Multi-threaded program to zero a block of data
  *
  * Compile with
  *    > gcc -g -Wall -Werror -D_POSIX_THREAD_SEMANTICS bzero.c -c -o bzero.o
  *    > gcc -g -Wall -Werror -D_POSIX_THREAD_SEMANTICS thread.c -c -o thread.o
  *    > gcc -lpthread bzero.o thread.o -o bzero
  * Run with
  *    > ./bzero
  */
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "thread.h"

#define BUFSIZE 1000
static unsigned char zeroblock[BUFSIZE];
static unsigned char testblock[BUFSIZE];

int main(int argc, char **argv) {
  // first, initialize the testblock to a known value
  (void) memset(testblock, 0x55, BUFSIZE);
  (void) memset(zeroblock, 0x0, BUFSIZE);

  // test the parallel block zero
  blockzero(testblock, BUFSIZE);

  // compare against the known zero block
  if (memcmp(testblock, zeroblock, BUFSIZE) == 0) 
    printf("Success!\n");
  else
    printf("block zero failed!\n");
}

// To pass two arguments, we need a struct to hold them.
typedef struct bzeroparams {
    unsigned char *buffer;
    int length;
};

#define NTHREADS 10

// Zero a block of memory using multiple threads.
void blockzero (unsigned char *p, int length) {
    int i, j;
    thread_t threads[NTHREADS];
    struct bzeroparams params[NTHREADS];

// For simplicity, assumes length is divisible by NTHREADS.
    assert((length % NTHREADS) == 0);
    for (i = 0, j = 0; i < NTHREADS; i++, j += length/NTHREADS) {
        params[i].buffer = p + i * length/NTHREADS;
        params[i].length = length/NTHREADS;
        thread_create_p(&(threads[i]), &go, &params[i]);
    }
    for (i = 0; i < NTHREADS; i++) {
        thread_join(threads[i]);
    }
}

void go (struct bzeroparams *p) {
    memset(p->buffer, 0, p->length);
    thread_exit(0);
    // Not reached
}
