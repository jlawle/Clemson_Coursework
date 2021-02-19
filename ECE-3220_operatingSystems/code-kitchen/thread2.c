/* Thread stuff */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>


void* t1(void * arg){
  int i,j;
    for(i=0; i<98000; i++){
      printf("1\n");
        for(j=0; j<10000000; j++){
        }
      }
  return NULL;
}

void t2(){
  int i,j;
  for(i=0; i<9800   0; i++){
    printf("2\n");
      for(j=0; j<10000000; j++){
      }
    }
}

int main() {
    pthread_t thread1;
    pthread_create(&thread1, NULL, t1, NULL);

    t2();
    return 0;
}
