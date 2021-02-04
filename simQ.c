#include <simQ.h>

int main (int argc, char **argv)
{
    printf("A");
    char * inputFileName = argv[1];
    int numSims = atoi(argv[2]);
    char * outputFileName = argv[3];

    printf("A");

    /* Simulation Parameters */
    int *simParams;
    simParams = (int *)malloc(sizeof(int)*8);

    printf("A");

    getSimParameters(inputFileName,
                     simParams);

    printf("B");

    int i;
    for (i=0; i < numSims; i++)
    {
        runSim(simParams);
    }
    return 0;
}

void runSim(int simParams[])
{
    unsigned int currentTime;
    int customersTotal = 0;
    int customersServed = 0;
    int customersTimedOut = 0;
    int customersUnfulfilled = 0;
    int customersAtServicePoint = 0;

    int maxQueueLength = simParams[0];
    int numServicePoints = simParams[1];
    int closingTime = simParams[2];
    int meanNewCustomers = simParams[3];
    int lowerLimitWaitTolerance = simParams[4];
    int upperLimitWaitTolerance = simParams[5];
    int scaleServeTime = simParams[6];
    int lowerLimitServeTime = simParams[7];

    SERVICEPOINT servicePoints[numServicePoints];
    int busyServicePoints = 0;

    NODE *customerQueue = NULL;
    customerQueue = (NODE *) malloc(sizeof(NODE));
    customerQueue->next = NULL;
    customerQueue->previous = NULL;
    customerQueue->waitLimit = INT_MIN;
    customerQueue->waitCurrent = INT_MIN;

    if (maxQueueLength == -1)
        maxQueueLength = INT_MAX;

    const gsl_rng_type *T;
    gsl_rng *r;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc(T);
    gsl_rng_set(r,time(0));

    for (currentTime=0; currentTime < closingTime; currentTime++)
    {
        printf("%d:\n", currentTime);

        /* Customers leave service point */
        customersServed += fulfillCustomer(&numServicePoints, servicePoints);

        /* Customers arrive at service point */
        startServingCustomer(&numServicePoints, servicePoints, customerQueue, *r, scaleServeTime, lowerLimitServeTime);

        /* Customer reaches wait limit */
        customersTimedOut += checkWaitLimit(&customerQueue);

        /* New Customers */
        unsigned int newCustomers = gsl_ran_poisson(r, meanNewCustomers);
        int i;
        for (i=0; i < newCustomers; i++)
        {
            if (size(customerQueue) < maxQueueLength)
            {
                int waitLimit = (int) gsl_ran_flat(r, lowerLimitWaitTolerance, upperLimitWaitTolerance);
                enqueue(customerQueue, waitLimit);
                customersTotal++;
            }
            else {
                customersUnfulfilled++;
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

        /* Increment the wait time of all customers in the queue and
        service points by 1 */
        updateWait(customerQueue);
        for (i=0; i < numServicePoints; i++)
        {
            if (servicePoints[i].id == 1)
                servicePoints[i].timeTaken++;
        }
    }

    printf("Post Office Close\n");

    while (customersAtServicePoint > 0)
    {
        printf("%d:\n", currentTime);

        /* Customers leave service point */
        customersServed += fulfillCustomer(&numServicePoints, servicePoints);

        /* Customers arrive at service point */
        startServingCustomer(&numServicePoints, servicePoints, customerQueue, *r, scaleServeTime, lowerLimitServeTime);

        /* Customer reaches wait limit */
        customersTimedOut += checkWaitLimit(&customerQueue);

        print_list(customerQueue);
        printf("Service Points:\n");
        customersAtServicePoint = 0;
        int i;
        for (i=0; i < numServicePoints; i++)
        {
            printf("%d, ", servicePoints[i].id);
            if (servicePoints[i].id == 1)
                customersAtServicePoint++;
        }
        printf("\n\n");

        /* Increment the wait time of all customers in the queue and
        service points by 1 */
        updateWait(customerQueue);
        for (i=0; i < numServicePoints; i++)
        {
            if (servicePoints[i].id == 1)
                servicePoints[i].timeTaken++;
        }

        /* Increment Time interval as this loops after the post office has closed */
        currentTime++;
    }

    printf("Total Customers: %d\n", customersTotal);
    printf("Customers Served: %d\n", customersServed);
    printf("Customers Timed-out: %d\n", customersTimedOut);
    printf("Customers Unfulfilled: %d\n", customersUnfulfilled);
    printf("Customers at Service Points: %d\n", customersAtServicePoint);
    printf("Total Time: %d\n", currentTime);
    printf("Size of Queue: %d\n", size(customerQueue));
    print_list(customerQueue);
}

int fulfillCustomer(int *numServicePoints, SERVICEPOINT servicePoints[])
{
    int customersServed = 0;
    int i;
    for (i=0; i < *numServicePoints; i++)
    {
        if (servicePoints[i].timeTaken == servicePoints[i].timeDone && servicePoints[i].id == 1)
        {
            printf("Customer Served.\n");
            customersServed++;
            servicePoints[i].id = 0;
        }
    }
    return customersServed;
}

void startServingCustomer(int *numServicePoints, SERVICEPOINT servicePoints[], NODE customerQueue[], gsl_rng r,  int scaleServeTime, int lowerLimitServeTime)
{
    int i;
    for (i=0; i < *numServicePoints; i++)
    {
        if (servicePoints[i].id != 1)
        {
            NODE * customer = dequeue(&customerQueue);
            if (customer) {
                SERVICEPOINT servicePoint;
                servicePoint.timeTaken = 0;
                int timeToServe = (int) gsl_ran_rayleigh_tail(&r, scaleServeTime, lowerLimitServeTime);
                servicePoint.timeDone = timeToServe;
                servicePoint.id = 1;
                servicePoints[i] = servicePoint;
            }
        }
    }
}
