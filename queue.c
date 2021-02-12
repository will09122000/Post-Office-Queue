#include <queue.h>

/*
    This file contains functions that handle the post office queue in the form
    of a linked list.
*/

/*
    Function: size
    ---------------------------------------------------------------------------
    Computes the size of the post office queue.

    head:    a pointer to the start of the list

    returns: the size of the list
*/
unsigned int size(NODE * head)
{
    unsigned int count = 0;
    NODE *current = head;
    while (current != NULL) 
    { 
        count++; 
        current = current->next; 
    } 
    return count-1; 
}

/*
    Function: enequeue
    ---------------------------------------------------------------------------
    Adds a new customer to the end of the post office queue.

    head:      a pointer to the start of the list
    waitLimit: the maximum time the new customer is willing to wait

    returns:   null
*/
void enqueue(NODE * head, int waitLimit)
{
    NODE * current = head;

    while (current->next != NULL)
        current = current->next;

    current->next = (NODE *) malloc(sizeof(NODE));
    current->next->waitLimit = waitLimit;
    current->next->waitCurrent = 0;
    current->next->next = NULL;
    current->next->previous = current;
}

/*
    Function: dequeue
    ---------------------------------------------------------------------------
    Removes the customer at the front of the post office queue.

    head:    a double pointer to the start of the list

    returns: the length of time the customer has been waiting in the queue
*/
unsigned int dequeue(NODE ** head)
{
    NODE * nodeRemoved;
    NODE * nextNode = NULL;
    unsigned int waitTime = nodeRemoved->waitCurrent;

    switch (size(*head))
    {
    case 0:
        return;

    case 1:
        nodeRemoved = (*head)->next;
        waitTime = nodeRemoved->waitCurrent;
        free((*head)->next);
        (*head)->next = NULL;
        break;

    default:
        nextNode = (*head)->next->next;
        nodeRemoved = (*head)->next;
        (*head)->next = nodeRemoved->next;
        nextNode->previous = *head;
        waitTime = nodeRemoved->waitCurrent;
        free(nodeRemoved);
        break;
    }

    return waitTime;
}

/*
    Function: updateWait
    ---------------------------------------------------------------------------
    Increments the wait time of each customer in the queue by 1.

    head:    a pointer to the start of the list

    returns: null
*/
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

/*
    Function: checkWaitLimit
    ---------------------------------------------------------------------------
    Iterates through the customer queue and removes customers that have reached
    their wait limit.

    head:    a double pointer to the start of the list

    returns: the number of customers that have timed-out during the
             current time unit
*/
unsigned int checkWaitLimit(NODE ** head)
{
    NODE * current = *head;
    unsigned int newTimedOutCustomers = 0;

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
        
            /* If node is head node, go to next node. */
            if (*head == current) 
                *head = current->next; 
        
            /* Change 'next' pointer as long as the node to be deleted is not the
               last node */
            if (current->next != NULL) 
                current->next->previous = current->previous; 
        
            /* Change 'previous' pointer as long as the node to be deleted is not
               the first node */
            if (current->previous != NULL) 
                current->previous->next = current->next; 
        
            free(current);
            newTimedOutCustomers++;
        }
        current = current->next;
    }
    return newTimedOutCustomers;
}

/*
    Function: freeQueue
    ---------------------------------------------------------------------------
    Frees the memory taken up by the post office queue.

    head:    a double pointer to the start of the list

    returns: null
*/
void freeQueue (NODE ** head)
{
   NODE *current = *head;
   while ( *head != NULL )
   {
      current = (*head)->next;
      free(*head);
      *head = current;
   }
}