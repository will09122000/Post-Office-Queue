/* C program for array implementation of queue */
#include <limits.h>
#include <stdlib.h>

typedef struct Queue QUEUE;

/* A structure to represent a queue */
struct Queue {
    int front, rear, size;
    unsigned maxLength;
    int* array;
};

/* function to create a queue of given maxLength.
It initializes size of queue as 0 */
QUEUE* createQueue(unsigned maxLength)
{
    QUEUE* queue = (QUEUE*)malloc(sizeof(QUEUE));
    queue->maxLength = maxLength;
    queue->front = queue->size = 0;

    /* This is important, see the enqueue */
    queue->rear = maxLength - 1;
    queue->array = (int*)malloc(queue->maxLength * sizeof(int));
    return queue;
}

/* Queue is empty when size is 0 */
int isEmpty(QUEUE* queue)
{
    return (queue->size == 0);
}

/* Queue is full when size becomes
equal to the maxLength */
int isFull(QUEUE* queue)
{
    return (queue->size == queue->maxLength);
}

/* Function to add an item to the queue.
It changes rear and size */
void enqueue(QUEUE* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->maxLength;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

/* Function to remove an item from queue.
It changes front and size  */
int dequeue(QUEUE* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->maxLength;
    queue->size = queue->size - 1;
    return item;
}

/* Function to get front of queue */
int front(QUEUE* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}
