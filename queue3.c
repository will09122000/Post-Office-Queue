#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct node {
    int waitLimit, currentWait;
    struct node * next;
} NODE;

/*
int size(NODE * head);
void print_list(NODE * head);
void enqueue(NODE * head, int val);
int pop(NODE ** head);
int remove_by_index(NODE ** head, int n);

int main()
{
    NODE * head = NULL;
    head = (NODE *) malloc(sizeof(NODE));
    head->next = NULL;
    head->waitLimit = INT_MIN;
    head->currentWait = INT_MIN;
    
    printf("Size: %d\n", size(head));
    enqueue(head, 3);
    enqueue(head, 4);
    enqueue(head, 5);
    enqueue(head, 6);
    enqueue(head, 7);
    printf("Size: %d\n", size(head));
    print_list(head);
    printf("Popped: %d\n", pop(&head));
    printf("Size: %d\n", size(head));
    print_list(head);
    printf("Removed: %d\n", remove_by_index(&head, 2));
    print_list(head);
    printf("Size: %d\n", size(head));

    return 0;
}
*/

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
    int i = 0;
    int retval = -1;
    NODE * current = *head;
    NODE * temp_node = NULL;

    if (n == 0) {
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

void checkWaitLimit(NODE * head)
{
    NODE * current = head;
    int count = 1;

    if (current->waitLimit == INT_MIN)
    {
        current = current->next;
    }

    while (current != NULL)
    {
        if (current->currentWait >= current->waitLimit)
        {
            remove_by_index(&head, count);
        }

        if (current == NULL)
            break;

        current = current->next;
        count++;
    }

}