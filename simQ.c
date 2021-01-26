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
    unsigned int currentTime = 0;

    struct Queue* queue = createQueue(maxQueueLength);
    struct Queue* servicePoints = createQueue(numServicePoints);

    const gsl_rng_type *T;
    gsl_rng *r;
    /* create a random number generator */
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc(T);
    /* seed it – equivalent of srand(time(0)) */
    gsl_rng_set(r,time(0));

    while (time < closingTime)
    {
        printf("%2d: %lf\n", i, gsl_ran_flat(r,2,5));
        currentTime++;
    }

    enqueue(queue, 10);
    enqueue(queue, 20);
    enqueue(queue, 30);
    enqueue(queue, 40);
    dequeue(queue);

    printf("Front item is %d\n", front(queue));
}