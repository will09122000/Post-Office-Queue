#include <limits.h>
#include <stdlib.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>



struct Node 
{ 
    int waitLimit, currentWait; 
    struct Node* next; 
}; 
typedef struct Node NODE;

void enqueue(NODE** head_ref, int waitLimit)
{
    NODE *newNode;
    if ( ( newNode = (NODE *)malloc(sizeof(NODE)) ) == NULL )
    {
        newNode->waitLimit = waitLimit; 
        newNode->currentWait = 0; 
        newNode->next = (*head_ref); 
        (*head_ref)    = newNode;
    }
    else
    {
        printf("NULL");
    }
}

int size(NODE* head) 
{ 
    int count = 0;
    NODE* current = head;
    while (current != NULL) 
    { 
        count++; 
        current = current->next; 
    } 
    return count; 
}