#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "queue.c"

typedef struct Queue QUEUE;

void runSim();

int main (int argc, char **argv)
{
    char * inputFileName = argv[1];
    int numSims = atoi(argv[2]);
    char * outputFileName = argv[3];

    printf("%s\n", inputFileName);
    printf("%d\n", numSims);
    printf("%s\n", outputFileName);

    int i;
    for (i=0; i < numSims; i++)
    {
        runSim();
    }

    return 0;
}

void runSim()
{
    unsigned int maxQueueLength = 10;
    unsigned int numServicePoints = 3;
    unsigned int closingTime = 100;
    float mean = 5;
    float standardDeviation = 2;
    unsigned int time = 0;

    struct Queue* queue = createQueue(maxQueueLength);
    struct Queue* servicePoints = createQueue(numServicePoints);

    while (time < closingTime)
    {
        int randomTime = rand() % 5;
        printf("Random num: %d\n", randomTime);
        time++;
    }

    enqueue(queue, 10);
    enqueue(queue, 20);
    enqueue(queue, 30);
    enqueue(queue, 40);
    dequeue(queue);

    printf("Front item is %d\n", front(queue));
}