#include <simQ.h>

int main (int argc, char **argv)
{
    char * inputFileName = argv[1];
    int numSims = atoi(argv[2]);
    char * outputFileName = argv[3];


    FILE *fp;
    int maxQueueLength, numServicePoints, closingTime, meanNewCustomers, stanDevNewCustomers, meanWaitTolerance, standDevWaitTolerance;
    if ( (fp = fopen(inputFileName, "r")) == NULL )
    {
        printf("Unable to open %s\n", inputFileName);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        exit(1);
    }

    fscanf(fp, "maxQueueLength: %d\nnumServicePoints: %d\nclosingTime: %d\nmeanNewCustomers: %d\nstanDevNewCustomers: %d\nmeanWaitTolerance: %d\nstandDevWaitTolerance: %d\n", maxQueueLength, numServicePoints, closingTime, meanNewCustomers, stanDevNewCustomers, meanWaitTolerance, standDevWaitTolerance);

/*
    while ( !feof(fp) )
    {
        if(fgets(fileLine, 60, fp) != NULL);
        {
            printf("%s",fileLine);
            sscanf(fileLine, "%d", parameters[i]);
            i++;
        }
            
    }
    */
    printf("\n");

    /*
    int i;
    for (i=0; i < numSims; i++)
    {
        runSim();
    }
    */


    return 0;
}

void runSim()
{
    unsigned int maxQueueLength = 20;
    unsigned int numServicePoints = 3;
    unsigned int closingTime = 10;
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

        /*fulfillCustomers(&numServicePoints, servicePoints, &customersServed);*/
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
        
        /* New Customers */
        unsigned int newCustomers = gsl_ran_poisson(r, 2);
        for (i=0; i < newCustomers; i++)
        {
            if (size(customerQueue) < maxQueueLength)
            {
                int waitLimit = (int)gsl_ran_flat(r,3,5);
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
