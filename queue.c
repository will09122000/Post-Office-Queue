/* C program for array implementation of queue */
#include <limits.h>
#include <stdlib.h>

typedef struct Queue QUEUE;
typedef struct Customer CUSTOMER;

struct Customer {
    int waitLimit, currentWait;
};

/* A structure to represent a queue */
struct Queue {
    int front, rear, size;
    unsigned maxLength;
    CUSTOMER* array;
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
    queue->array = (CUSTOMER*)malloc(queue->maxLength * sizeof(CUSTOMER));
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
void enqueue(QUEUE* queue, CUSTOMER item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->maxLength;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

/* Function to remove an item from queue.
It changes front and size  */
CUSTOMER dequeue(QUEUE* queue)
{
    /*
    if (isEmpty(queue))
        return INT_MIN;
    */
    CUSTOMER item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->maxLength;
    queue->size = queue->size - 1;
    return item;
}

/* Function to get front of queue */
CUSTOMER front(QUEUE* queue)
{
    /*
    if (isEmpty(queue))
        return INT_MIN;
    */
    return queue->array[queue->front];
}

void updateWait(QUEUE* queue)
{
    int i;
    int queueSize = queue->size;
    for (i=0; i < queueSize; i++)
    {
        queue->array[i].currentWait++;
    }
}

void waitLimitReached(QUEUE* queue)
{
    int i, j;
    int queueSize = queue->size;
    for (i=0; i < queue->size; i++)
    {
        if (queue->array[i].currentWait == queue->array[i].waitLimit)
        {
            printf("Position: %d\n", i);

            if ((queue->front == i))
            {
                CUSTOMER boredCustomer = dequeue(queue);
                printf("Bored Customer at front\n");
            }
            else if (queue->rear == i)
            {
                CUSTOMER boredCustomer = queue->array[i];
                queue->rear = (queue->rear - 1) % queue->maxLength;
                queue->size = queue->size - 1;
                printf("Bored Customer at rear\n");
            } else {
                CUSTOMER boredCustomer = queue->array[i];
                queue->rear = (queue->rear - 1) % queue->maxLength;
                queue->size = queue->size - 1;
                for(j=i; j<queue->size-1; j++)
                    queue->array[j] = queue->array[j + 1];
                printf("Bored Customer in middle\n");
            }
        }
    }
}