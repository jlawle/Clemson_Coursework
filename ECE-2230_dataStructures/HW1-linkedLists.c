/*
  John Lawler
  ECE-2230-001
  September 3 2019
  C program to create singly linked list of numbers and perform various
  operations on them.

*/

//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

typedef struct listNode {
  int num;
  struct listNode *next;
} numNode;


//-----------------------------------------------------------------------------

/* This function is used to print the list. It parses through and prints each
   node data point. */
void printList(numNode *Head){
  numNode *temp = Head;
  if(Head != NULL){
    do{
      if(temp->num > 0 && temp->num < 30) //adjusts for overflow or leakage
        printf("%d ", temp->num);
      temp = temp->next;
    } while(temp != Head);
  }
}

//-----------------------------------------------------------------------------
/* This function takes in the head node, and a new node with the new data
    wanting to insert. it uses a key to parse through through the list to find
    what node it should enter after. */
void insertNode(numNode **Head, numNode* newNode, int key){
  numNode* current = *Head;
  while(current->num != key){
    current = current->next;
  }
  newNode->next = current->next;
  current->next = newNode;
}

//-----------------------------------------------------------------------------

/* this function adds nodes to the start of the list. it takes in the last NULL
  node pointer and places the data number in front of it. */
void addNode(numNode **Head, int number){
  numNode *ptr = (numNode*)malloc(sizeof(numNode));
  numNode *temp = *Head;
  ptr->num = number;
  ptr->next = *Head;

  if(*Head != NULL){ //checks if the list is a single node or not
    while(temp->next != *Head){
      temp = temp->next;
    }
    temp->next = ptr;
  }
  else { //creates the first node if list is empty
    ptr->next = ptr;
  }
  *Head = ptr;
}

//-----------------------------------------------------------------------------

/* this function takes in the head pointer to a node and a number to search
    for. After finding the node it frees the node it is currently on. */
void deleteNode(numNode* Head, int number){
  if (Head == NULL) return; //check if empty list, theres nothing to delete
  numNode *current = Head, *prev;
  //while loop to find the number to delete
  while(current->num != number){
    if(current->next == Head){
      printf("No Node found\n");
      break;
    }
    prev = current;
    current = current->next;
  }
  //if the linked list is only one node long
  if(current->next == Head){
    Head = NULL;
    free(current);
    return;
  }
  //if the node of interest starts at the head of the linked lisyt
  if(current == Head){
    prev->next = Head->next;
    Head = current->next;
    free(current);
  }
  //handle for if the node is at the end of the list
  else if (current->next == Head){
    prev->next = Head;
    free(current);
  }
  //handle for if the node is located somewhere in the middle of the list
  else{
    prev->next = current->next;
    free(current);
  }
}

//-----------------------------------------------------------------------------

int main(){
  //variable declarations
  int i;
  int arr1[] = {1,2,3,4,5,6,7,8,9,10};
  int arr2[] = {21,22,23,24,25,26,27,28,29,30};
  int arr3[] = {11,12,13,14,15,16,17,18,19,20};
  numNode *head = NULL;
  numNode *temp;

  //loop to create a linked list with 1-10 at the head and 21-30 at the tail
  for(i=9; i>=0; i--){
    addNode(&head,arr2[i]);
  }
  for(i=0; i<=9; i++){
    addNode(&head,arr1[i]);
  }

  //for loop backwards to insert 11-20 in the middle using the insert function
  for(i=9;i>=0;i--){
    temp = (numNode*)malloc(sizeof(numNode));
    temp->num = arr3[i];
    insertNode(&head,temp,1);
  }

  printf("Linked List Before Deletion: \n");
  printList(head);
  //for loops to delete the first 5 nodes and the last 5
  for(i=6;i<11;i++) deleteNode(head,i);
  for(i=26;i<31;i++) deleteNode(head,i);

  printf("\n");
  printf("\nLinked List After Deletion: \n");
  printList(head);
  printf("\n");
  return 0;
}

//-----------------------------------------------------------------------------
