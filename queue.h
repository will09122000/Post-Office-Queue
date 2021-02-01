#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

struct node
{
    int waitCurrent;
    int waitLimit;
    struct node * previous;
    struct node * next;
};

typedef struct node NODE;