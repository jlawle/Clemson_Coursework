/*
  John Lawler - Homework 2
  ECE 2230-001
  September 9 2019

  The purpose of this program is to write a program with a stack and queue. tthe
  numbers 1-20 are pushed onto the stack. They are popped off, then inserted into
  a queue. After, the numbers are removed and printed.
*/
//-----------------------------------------------------------------------------

#define SSIZE 20
#define QSIZE 20
#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int Count,Front,Rear, Items[QSIZE];
} Queue;

typedef struct {
  int Count, Items[SSIZE];
} Stack;

//-----------------------------------------------------------------------------
//intializes the q to zero
void InitializeQ(Queue *Q){
  Q->Count = 0; Q->Front = 0; Q->Rear = 0;
}

//checks if the queue is empty
int isQempty(Queue *Q){
  if(Q->Count == 0){
    return 1;
  } else return 0;
}

//checks to see if  the queue is full returns 1 if true
int isQfull(Queue *Q){
  if(Q->Count == QSIZE){
    return 1;
  } else return 0;
}

//inserts an item onto the queue
void Insert(Queue *Q, int item){
  if(Q->Count == QSIZE){
    printf("\nError: insertion into full queue\n");
    return;
  } else{
    Q->Items[Q->Rear] = item;
    Q->Rear = (Q->Rear + 1) % QSIZE;
    ++(Q->Count);
  }
}
void Remove(Queue *Q, int *item){
  if(Q->Count == 0){
    printf("Error: system is already empty.");
    return;
  } else {
    *item = Q->Items[Q->Front];
    Q->Front = (Q->Front + 1) % QSIZE;
    --(Q->Count);
  }
}

//-----------------------------------------------------------------------------
//creates the Stack
void InitializeS(Stack *S){
  S->Count = 0;
}
//checks if stack is empty, returns 1 if true
int isSempty(Stack *S){
  if(S->Count == 0){
    return 1;
  } else return 0;
}
//checks if stack is full returns 1 if true
int isSfull(Stack *S){
  if(S->Count == SSIZE){
    return 1;
  } else return 0;
}
//pops an item off the stack using a pointer to save its location in mem
void Pop(Stack *S, int *num){
  if(S->Count == 0){
    printf("error: attempt to pop empty stack\n");
    return;
  } else {
    --(S->Count);
    *num = S->Items[S->Count];
  }
}
//pushes new item onto the stack
void Push(Stack *S, int num){
  if(S->Count == SSIZE){
    printf("error: pushing new item onto full stack\n");
    return;
  } else {
    S->Items[S->Count] = num;
    ++(S->Count);
  }
}
//-----------------------------------------------------------------------------
int main(){
  Stack *stack;
  stack = (Stack*)malloc(sizeof(Stack));
  Queue *queue;
  queue = (Queue*)malloc(sizeof(Queue));
  int i, num, ssize, qsize;
  InitializeS(stack);
  InitializeQ(queue);
  printf("\nPushing numbers 1-20 onto stack...\n");
  for(i=1;i<=20;i++){
    Push(stack,i);
  }
  ssize = stack->Count;
  printf("current stack Size: %d \n",stack->Count);
  printf("current queue Size: %d \n",queue->Count);

  printf("\nPopping the stack and inserting to queue...\n");
  while(ssize != 0){
    Pop(stack, &num);
    Insert(queue, num);
    ssize = stack->Count;
  }
  printf("current stack Size: %d \n",stack->Count);
  printf("current queue Size: %d \n",queue->Count);

  printf("\nRemoving items and printing...\n");
  qsize = queue->Count;
  while(qsize != 0){
    Remove(queue, &num);
    printf("%d ",num);
    qsize = queue->Count;
  }
  printf("\n\n");
  return 0;
}


