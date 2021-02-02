#include <simQ.h>

int main (int argc, char **argv)
{
    char * inputFileName = argv[1];
    int numSims = atoi(argv[2]);
    char * outputFileName = argv[3];

    // Simulation Parameters
    int maxQueueLength;
    int numServicePoints;
    int closingTime;
    int meanNewCustomers;
    int stanDevNewCustomers;
    int meanWaitTolerance;
    int standDevWaitTolerance;

    getSimParameters(inputFileName,
                     &maxQueueLength,
                     &numServicePoints,
                     &closingTime,
                     &meanNewCustomers,
                     &stanDevNewCustomers,
                     &meanWaitTolerance,
                     &standDevWaitTolerance);

    int i;
    for (i=0; i < numSims; i++)
    {
        runSim(maxQueueLength,
               numServicePoints,
               closingTime,
               meanNewCustomers,
               stanDevNewCustomers,
               meanWaitTolerance,
               standDevWaitTolerance);
    }

    return 0;
}

void runSim(int maxQueueLength,
            int numServicePoints,
            int closingTime,
            int meanNewCustomers,
            int stanDevNewCustomers,
            int meanWaitTolerance,
            int standDevWaitTolerance)
{
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

        /*fulfillCustomers(&numServicePoints, servicePoints, &customersServed);*/
        /* Customers leave service point */
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

        /* Customers arrive at service point */
        startServingCustomer(&numServicePoints, servicePoints, &customerQueue);


        /* Customer reaches wait limit */
        customersBored += checkWaitLimit(&customerQueue);
        
        /* New Customers */
        unsigned int newCustomers = gsl_ran_poisson(r, 1);
        for (i=0; i < newCustomers; i++)
        {
            if (size(customerQueue) < maxQueueLength)
            {
                int waitLimit = (int)gsl_ran_flat(r,2,4);
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

    printf("Post Office Close\n");

    while (customersAtServicePoint > 0)
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
        customersBored += checkWaitLimit(&customerQueue);

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

void startServingCustomer(int numServicePoints, SERVICEPOINT servicePoints[], NODE customerQueue[])
{
    int i;
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
}