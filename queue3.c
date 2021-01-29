#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

struct node
{
    int waitLimit, currentWait;
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

void print_list(NODE * head) {
    NODE * current = head;


    if (current->waitLimit == INT_MIN) {
        current = current->next;
    }


    while (current != NULL) {
        printf("Wait Limit: %d Current Wait: %d\n", current->waitLimit, current->currentWait);
        current = current->next;
    }
    printf("\n");
}

void enqueue(NODE * head, int waitLimit) {
    NODE * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    /* now we can add a new variable */
    current->next = (NODE *) malloc(sizeof(NODE));
    current->next->waitLimit = waitLimit;
    current->next->currentWait = 0;
    current->next->next = NULL;
    current->next->previous = current;
}

int pop(NODE ** head) {
    int retval = -1;
    NODE * next_node = NULL;

    if (*head == NULL) {
        return -1;
    }

    if ((*head)->waitLimit == INT_MIN)
        *head = (*head)->next;

    next_node = (*head)->next;
    retval = (*head)->waitLimit;
    free(*head);
    *head = next_node;

    return retval;
}

int remove_by_index(NODE ** head, int n) {
    n++;
    int i = 0;
    int retval = -1;
    NODE * current = *head;
    NODE * temp_node = NULL;

    if (n == 1) {
        return pop(head);
    }

    for (i = 0; i < n-1; i++) {
        if (current->next == NULL) {
            return -1;
        }
        current = current->next;
    }

    temp_node = current->next;
    retval = temp_node->waitLimit;
    current->next = temp_node->next;
    free(temp_node);

    return retval;

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
        current->currentWait++;
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
        if (current->currentWait >= current->waitLimit)
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
