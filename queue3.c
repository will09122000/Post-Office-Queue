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

int size(NODE * head)
{
    int count = 0;
    NODE *current = head;
    while (current != NULL) 
    { 
        count++; 
        current = current->next; 
    } 
    return count-1; 
}

void print_list(NODE * head)
{
    NODE * current = head;

    if (current->waitLimit == INT_MIN)
        current = current->next;

    while (current != NULL)
    {
        printf("Wait Limit: %d Current Wait: %d\n", current->waitLimit, current->waitCurrent);
        current = current->next;
    }
    printf("\n");
}

void enqueue(NODE * head, int waitLimit)
{
    NODE * current = head;

    while (current->next != NULL)
        current = current->next;

    /* now we can add a new variable */
    current->next = (NODE *) malloc(sizeof(NODE));
    current->next->waitLimit = waitLimit;
    current->next->waitCurrent = 0;
    current->next->next = NULL;
    current->next->previous = current;
}

NODE* dequeue(NODE ** head)
{
    NODE* nodeRemoved;
    NODE * next_node = NULL;

    if (size(*head) == 0)
        return;


    next_node = (*head)->next;
    nodeRemoved = *head;
    free(*head);
    *head = next_node;

    return nodeRemoved;
}

void updateWait(NODE * head)
{
    NODE * current = head;

    if (current->waitLimit == INT_MIN)
    {
        current = current->next;
    }

    while (current != NULL)
    {
        current->waitCurrent++;
        current = current->next;
    }

}

void checkWaitLimit(NODE ** head)
{
    NODE * current = *head;

    if (size(*head) == 0)
        return;

    if (current->waitLimit == INT_MIN)
        current = current->next;

    while (current != NULL)
    {
        if (current->waitCurrent >= current->waitLimit)
        {
            if (*head == NULL || current == NULL) 
                return; 
        
            /* If node to be deleted is head node */
            if (*head == current) 
                *head = current->next; 
        
            /* Change next only if node to be 
            deleted is NOT the last node */
            if (current->next != NULL) 
                current->next->previous = current->previous; 
        
            /* Change previous only if node to be 
            deleted is NOT the first node */
            if (current->previous != NULL) 
                current->previous->next = current->next; 
        
            /* Finally, free the memory occupied by del*/
            free(current);
            printf("Customer Removed.\n");
        }
        current = current->next;
    }
}
