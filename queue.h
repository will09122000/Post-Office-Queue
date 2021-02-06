#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

/*
    A struct that represents a customer in the queue.
*/
struct node
{
    int waitCurrent;
    int waitLimit;
    struct node * previous;
    struct node * next;
};

typedef struct node NODE;
