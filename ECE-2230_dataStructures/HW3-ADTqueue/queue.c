/* John Lawler
Homework 3 - ece 2230
Programming a queue wuth interface and test

this file comtains the main driver for the homework and all of the
functions */

#include "queue.h"
typedef int qData;
static QNode *head, *tail; //pointers to head and tail

//initialze the queue to NULL
void initQ(){
  head = tail = NULL;
}
//-----------------------------------------------------------------------------

//this function is used to empty the queue
void ClearQ(){
  QNode *temp;
  while(head != NULL){
    temp = head;
    head = head->next;
    free(temp); //iterate from each node to free it
  }
  head = tail = NULL; //reset to empty queue
}

//-----------------------------------------------------------------------------

//this function is used to insert an intem into the queue
void insertQ(qData Item){
  if(isFull()) return; //cant add new node to full queue
  QNode *temp; //declares temp node
  temp = (QNode *)malloc(sizeof(QNode)); //make size for new node
  temp->item = Item;
  temp->next = NULL;
  if(head == NULL){
    head = tail = temp; //if this is the first node being added to queue
  } else {
    tail->next = temp;
    tail = temp;
  }
}

//-----------------------------------------------------------------------------

//this function is used to remove an item from the queue using pointers
int removeQ(){
  int item;
  QNode *temp;
  if(isEmpty()){ //if queue is empty there is nothing to remove
     return 0;
  } else { //if queue is not empty
     item = head->item;   //get the item to be removed at the head
     temp = head;
     head = head -> next; //move the head to the next most node
     free(temp);
     if(isEmpty()){
       head = tail = NULL;
     }
   }
  return item;
}

//-----------------------------------------------------------------------------

//these functions check for the size, the fullness is used arbitrarily
int isEmpty(){
  return(head == NULL);
}

int isFull(){
  return 0;
}
