#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

/* A struct that represents a customer in the queue. */
struct node
{
    int waitCurrent;
    int waitLimit;
    struct node * previous;
    struct node * next;
};

typedef struct node NODE;

/* Function prototypes */
unsigned int size(NODE * head);
void enqueue(NODE * head, int waitLimit);
unsigned int dequeue(NODE ** head);
void updateWait(NODE * head);
unsigned int checkWaitLimit(NODE ** head);
void freeQueue (NODE ** head);
