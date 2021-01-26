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
    unsigned int closingTime = 20;
    float mean = 5;
    float standardDeviation = 2;
    unsigned int currentTime;

    struct Queue* queue = createQueue(maxQueueLength);
    struct Queue* servicePoints = createQueue(numServicePoints);

    const gsl_rng_type *T;
    gsl_rng *r;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc(T);
    gsl_rng_set(r,time(0));

    for (currentTime=0; currentTime < closingTime; currentTime++)
    {
        printf("%d:\n", currentTime);

        /* New Customer */
        if (gsl_ran_flat(r,0,2) < 1)
        {
            CUSTOMER newCustomer;
            newCustomer.waitLimit = (int)gsl_ran_flat(r,5,20);
            enqueue(queue, newCustomer);
            printf("Customer Arrived, Wait Limit: %d\n", newCustomer.waitLimit);
        }

        /* Increment the wait time of all customers in the queue by 1 */
        updateWait(queue);
    }

    printf("Size of Queue: %d\n", queue->size);

    int i;
    int queueSize = queue->size;
    for (i=0; i < queueSize; i++)
    {
        CUSTOMER customer = dequeue(queue);
        printf("Customer: Wait Limit: %d, Current Wait: %d\n", customer.waitLimit, customer.currentWait);
    }
    
}