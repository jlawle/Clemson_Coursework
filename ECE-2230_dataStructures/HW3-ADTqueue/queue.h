/* John Lawler HW3 ECE 2230

  This is the header file for the queue ADT program.

  */


#ifndef QUEUE_H
#define QUEUE_H
#include <stdlib.h>
#include <stdio.h>


typedef int qData; //opaque type handler
typedef struct QNodeType{
  qData *item;
  struct QNodeType *next;
} QNode;
void initQ(); //intialize queue
void ClearQ();  //clear queue
void insertQ(qData Item);    //insert new item
int removeQ();    //remove items
int isEmpty();
int isFull();

#endif
