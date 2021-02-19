#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BLOCK 4096

int main(int argc, char **argv)
{
  static const int BIG_SEEK = 1024 * 1024 * 1024; /* 1GB into file */
  char b[BLOCK];
  int f;
  int ii;
  int got;

  f = creat("sparse.dat", S_IRWXU);
  assert(f >= 0);
  printf("create succeeds.\n");
  
  for(ii = 0; ii < BLOCK; ii++){
    b[ii] = 'S';
  }

  got = write(f, b, BLOCK);
  assert(got == BLOCK);
  printf("first write succeeds\n");

  got = lseek(f, BIG_SEEK, SEEK_SET);
  assert(got == BIG_SEEK);
  printf("seek succeeds\n");
  
  got = write(f, b, BLOCK);
  assert(got == BLOCK);
  printf("second write succeeds\n");
  
}
