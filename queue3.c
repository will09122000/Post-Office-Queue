#include <limits.h>
#include <stdlib.h>

typedef struct node {
    int val;
    struct node * next;
} node_t;

void print_list(node_t * head);
void push(node_t * head, int val);
int pop(node_t ** head);
int remove_by_index(node_t ** head, int n);

int main()
{
    node_t * head = NULL;
    head = (node_t *) malloc(sizeof(node_t));
    head->next = NULL;
    
    head->val = 1;
    head->next = (node_t *) malloc(sizeof(node_t));
    head->next->val = 2;
    head->next->next = NULL;
    

    push(head, 3);
    push(head, 4);
    push(head, 5);
    push(head, 6);
    push(head, 7);
    print_list(head);
    printf("Popped: %d\n", pop(head));
    print_list(head);
    printf("Popped: %d", remove_by_index(head, 2));
    print_list(head);

    return 0;
}

void print_list(node_t * head) {
    node_t * current = head;

    while (current != NULL) {
        printf("%d, ", current->val);
        current = current->next;
    }
    printf("\n");
}

void push(node_t * head, int val) {
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    /* now we can add a new variable */
    current->next = (node_t *) malloc(sizeof(node_t));
    current->next->val = val;
    current->next->next = NULL;
}

int pop(node_t ** head) {
    int retval = -1;
    node_t * next_node = NULL;

    if (*head == NULL) {
        return -1;
    }

    next_node = (*head)->next;
    retval = (*head)->val;
    free(*head);
    *head = next_node;

    return retval;
}

int remove_by_index(node_t ** head, int n) {
    int i = 0;
    int retval = -1;
    node_t * current = *head;
    node_t * temp_node = NULL;

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
    retval = temp_node->val;
    current->next = temp_node->next;
    free(temp_node);

    return retval;

}