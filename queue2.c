#include <limits.h>
#include <stdlib.h>

struct Node 
{ 
    int waitLimit, currentWait; 
    struct Node* next; 
}; 
typedef struct Node NODE;

void enqueueFirst(NODE *root, int waitLimit)
{
        printf("First Customer Arrived, Wait Limit: %d\n", waitLimit);

    root = (NODE*) malloc(sizeof(NODE));
    (*root).waitLimit = waitLimit;
    (*root).currentWait = 0;
    (*root).next = NULL;
}

void enqueue(NODE *root, int waitLimit)
{
    NODE *currentNode = root;
    while (root->next != NULL) {
        currentNode = currentNode->next;
    }
    NODE* nextNode = (NODE*) malloc(sizeof(NODE));
    (*nextNode).waitLimit = waitLimit;
    (*nextNode).currentWait = 0;
    (*nextNode).next = NULL;
    (*currentNode).next = currentNode;
    printf("Customer Arrived, Wait Limit: %d\n", waitLimit);
}

int size(NODE *root) 
{ 
    int count = 0;
    NODE *current = root;
    while (current != NULL) 
    { 
        count++; 
        current = current->next; 
    } 
    return count; 
}

void updateWait(NODE *root)
{
    NODE *current = root;
    while (current != NULL) {
        current->currentWait++;
        current = current->next;
    }
}