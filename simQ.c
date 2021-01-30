#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "queue3.c"

struct servicePoint
{
    int timeTaken;
    int timeDone;
    int id;
};
typedef struct servicePoint SERVICEPOINT;

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

    int customersTotal = 0;
    int customersServed = 0;
    int customersBored = 0;
    int customersAtServicePoint = 0;

    SERVICEPOINT servicePoints[numServicePoints];
    int busyServicePoints = 0;

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

        int i;
        for (i=0; i < numServicePoints; i++)
        {
            if (servicePoints[i].timeTaken == servicePoints[i].timeDone && servicePoints[i].id == 1)
            {
                printf("Customer Served.\n");
                customersServed++;
                servicePoints[i].id = 0;
            }
        }


        for (i=0; i < numServicePoints; i++)
        {
            if (servicePoints[i].id != 1)
            {
                NODE * customer = dequeue(&customerQueue);
                if (customer) {
                    SERVICEPOINT servicePoint;
                    servicePoint.timeTaken = 0;
                    servicePoint.timeDone = 5;
                    servicePoint.id = 1;
                    servicePoints[i] = servicePoint;
                }
            }
        }

        /* Customer reaches wait limit */
        customersBored += checkWaitLimit(&customerQueue, customersBored);

        /* New Customers */
        if (gsl_ran_flat(r,0,2) < 1)
        {
            if (size(customerQueue) < maxQueueLength)
            {
                int waitLimit = (int)gsl_ran_flat(r,2,3);
                enqueue(customerQueue, waitLimit);
                customersTotal++;
                /*printf("Customer Arrived, Wait Limit: %d\n", waitLimit);*/
            }
            else {
                printf("Customer Rejected.");
            }
        }

        print_list(customerQueue);
        printf("Service Points:\n");
        customersAtServicePoint = 0;
        for (i=0; i < numServicePoints; i++)
        {
            printf("%d, ", servicePoints[i].id);
            if (servicePoints[i].id == 1)
                customersAtServicePoint++;
        }
        printf("\n\n");

        /* Increment the wait time of all customers in the queue by 1 */
        updateWait(customerQueue);

        for (i=0; i < numServicePoints; i++)
        {
            if (servicePoints[i].id == 1)
                servicePoints[i].timeTaken++;
        }
    }

    while (size(customerQueue) > 0 && customersAtServicePoint > 0)
    {
        printf("Post Office Close\n");
        printf("%d:\n", currentTime);

        int i;
        for (i=0; i < numServicePoints; i++)
        {
            if (servicePoints[i].timeTaken == servicePoints[i].timeDone && servicePoints[i].id == 1)
            {
                printf("Customer Served.\n");
                customersServed++;
                servicePoints[i].id = 0;
            }
        }


        for (i=0; i < numServicePoints; i++)
        {
            if (servicePoints[i].id != 1)
            {
                NODE * customer = dequeue(&customerQueue);
                if (customer) {
                    SERVICEPOINT servicePoint;
                    servicePoint.timeTaken = 0;
                    servicePoint.timeDone = 5;
                    servicePoint.id = 1;
                    servicePoints[i] = servicePoint;
                }
            }
        }

        /* Customer reaches wait limit */
        customersBored += checkWaitLimit(&customerQueue, customersBored);

        print_list(customerQueue);
        printf("Service Points:\n");
        customersAtServicePoint = 0;
        for (i=0; i < numServicePoints; i++)
        {
            printf("%d, ", servicePoints[i].id);
            if (servicePoints[i].id == 1)
                customersAtServicePoint++;
        }
        printf("\n\n");

        /* Increment the wait time of all customers in the queue by 1 */
        updateWait(customerQueue);

        for (i=0; i < numServicePoints; i++)
        {
            if (servicePoints[i].id == 1)
                servicePoints[i].timeTaken++;
        }

        currentTime++;
    }

    printf("Total Customers: %d\n", customersTotal);
    printf("Customers Served: %d\n", customersServed);
    printf("Customers Bored: %d\n", customersBored);
    printf("Customers at Service Points: %d\n", customersAtServicePoint);
    printf("Total Time: %d\n", currentTime);

    printf("Size of Queue: %d\n", size(customerQueue));
    print_list(customerQueue);

}