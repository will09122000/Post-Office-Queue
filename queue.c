#include <queue.h>

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
    printf("Queue:\n");

    if (current->waitLimit == INT_MIN)
        current = current->next;

    while (current != NULL)
    {
        printf("Wait Limit: %d Current Wait: %d\n", current->waitLimit, current->waitCurrent);
        current = current->next;
    }
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
    NODE * nextNode = NULL;

    switch (size(*head))
    {
    case 0:
        return;

    case 1:
        nodeRemoved = (*head)->next;
        free((*head)->next);
        (*head)->next = NULL;
        return nodeRemoved;

    default:
        nextNode = (*head)->next->next;
        nodeRemoved = (*head)->next;
        (*head)->next = nodeRemoved->next;
        nextNode->previous = *head;
        free(nodeRemoved);
        return nodeRemoved;
    }
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

int checkWaitLimit(NODE ** head)
{
    NODE * current = *head;
    int newBoredCustomers = 0;

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
            printf("Customer Bored.\n");
            newBoredCustomers++;
        }
        current = current->next;
    }
    return newBoredCustomers;
}
