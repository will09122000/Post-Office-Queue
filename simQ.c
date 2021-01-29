#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "queue3.c"

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
    unsigned int maxQueueLength = 20;
    unsigned int numServicePoints = 3;
    unsigned int closingTime = 20;
    float mean = 5;
    float standardDeviation = 2;
    unsigned int currentTime;

    NODE *customerQueue = NULL;
    customerQueue = (NODE *) malloc(sizeof(NODE));
    customerQueue->next = NULL;
    customerQueue->previous = NULL;
    customerQueue->waitLimit = INT_MIN;
    customerQueue->waitCurrent = INT_MIN;

    const gsl_rng_type *T;
    gsl_rng *r;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc(T);
    gsl_rng_set(r,time(0));

    for (currentTime=0; currentTime < closingTime; currentTime++)
    {
        printf("%d:\n", currentTime);

        /* Customer reaches wait limit */
        checkWaitLimit(&customerQueue);
        

        /* New Customers */
        if (gsl_ran_flat(r,0,2) < 1)
        {
            if (size(customerQueue) < maxQueueLength)
            {
                int waitLimit = (int)gsl_ran_flat(r,3,7);
                enqueue(customerQueue, waitLimit);
                printf("Customer Arrived, Wait Limit: %d\n", waitLimit);
            }
            else {
                printf("Customer Rejected.");
            }
        }

        print_list(customerQueue);
        /* Increment the wait time of all customers in the queue by 1*/
        updateWait(customerQueue); 
    }

    printf("Size of Queue: %d\n", size(customerQueue));
    print_list(customerQueue);
    NODE * test = dequeue(&customerQueue);
    NODE * tests = dequeue(&customerQueue);
    print_list(customerQueue);

}