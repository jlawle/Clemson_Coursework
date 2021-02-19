/* John Lawler
  ECE 2230 Homework 3
  program test

This file is used to send a test sequence useing my queue program in queue.c
and the header file
*/

#include "queue.h"
int main(){
    int test[5] = {1,2,3,4,5};
    char letters[5] = {'h','e','l','l','o'};
    int i;
    int test2 = 13;
    int item;
    initQ(); //initialize queue
    //inserting one item
    printf("Testing insert and removal of a single item\n");
    insertQ(test2);
    printf("Inserted and Removed: %d\n", removeQ());

    //inserting the array of items
    printf("\n\nTesting array [5] = 1 2 3 4 5, insertion starting at 1\n");
    for(i = 0; i<5; i++){
      insertQ(test[i]);
    }

    printf("\nDequeued letters are: \n");
    while((item = removeQ()) != 0) // removeQ() returns zero if empty
        printf("%i ", item);            // when queue is empty

    //testing character insertion and removal
    printf("\n\ninserting and removing letters: \n");
    for(i = 0; i < 5; i++){
      insertQ(letters[i]); //insert the string Hello into the queue
    }

    printf("\nRemoving letters: \n");
    while((item = removeQ()) != 0){
      printf("%c ", item);
    }
    printf("\n\n...Program Exit.\n");
    return 0;
}
