#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "queue2.c"


/*
typedef struct Queue QUEUE;
*/


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

    /*
    struct Queue* queue = createQueue(maxQueueLength);
    struct Queue* servicePoints = createQueue(numServicePoints);
    */
    NODE *root = NULL;
    root = (NODE*) malloc(sizeof(NODE));
    (*root).waitLimit = 5;
    (*root).currentWait = 0;
    (*root).next = NULL;

    const gsl_rng_type *T;
    gsl_rng *r;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc(T);
    gsl_rng_set(r,time(0));

    for (currentTime=0; currentTime < closingTime; currentTime++)
    {
        printf("%d:\n", currentTime);

        /* Customer reaches wait limit 
        checkWaitLimit(queue);
        */

        /* New Customers */
        if (gsl_ran_flat(r,0,2) < 1)
        {
            if (size(root) < maxQueueLength)
            {
                /*
                CUSTOMER newCustomer;
                newCustomer.waitLimit = (int)gsl_ran_flat(r,2,5);
                enqueue(queue, newCustomer);
                printf("Customer Arrived, Wait Limit: %d\n", newCustomer.waitLimit);
                */
                int waitLimit = (int)gsl_ran_flat(r,2,5);
                NODE *rootCopy = root;
                if (size(root) == 0)
                    enqueueFirst(root, waitLimit);
                else
                    enqueue(root, waitLimit);

                if (root->next == rootCopy->next)
                    printf("Equal");
            }
            else {
                printf("Customer Rejected.");
            }
        }

        /* Increment the wait time of all customers in the queue by 1
        updateWait(root); */
    }

    printf("Size of Queue: %d\n", size(root));

    /*
    int i;
    int queueSize = size(root);
    for (i=0; i < queueSize; i++)
    {
        NODE customer = dequeue(queue);
        printf("Customer: Wait Limit: %d, Current Wait: %d\n", customer.waitLimit, customer.currentWait);
    }
    */
}