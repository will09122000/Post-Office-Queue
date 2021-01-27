#include <limits.h>
#include <stdlib.h>

struct Node 
{ 
    int waitLimit, currentWait; 
    struct Node* next; 
}; 
typedef struct Node NODE;

void enqueue(NODE* head, int waitLimit)
{
    NODE *newNode;
    if ( ( newNode = (NODE *)malloc(sizeof(NODE)) ) == NULL )
    {
        newNode->waitLimit = waitLimit; 
        newNode->currentWait = 0; 
        newNode->next = NULL;
        head->next = newNode;
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

void updateWait(NODE* head)
{
    NODE * current = head;
    while (current != NULL) {
        current->currentWait++;
        current = current->next;
    }
}