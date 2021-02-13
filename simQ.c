#include <simQ.h>

/*
    This file contains the main function as well as functions that handle
    the simulation of a post office queue.
*/

int main (int argc, char **argv)
{
    validateInputs(argc, argv);

    /* Terminal Arguments */
    char * inputFileName = argv[1];
    unsigned int numSims = atoi(argv[2]);
    char * outputFileName = argv[3];

    /* Read, verify and write the Simulation Parameters */
    INPUT simParams = getSimParameters(inputFileName);
    writeSimParameters(inputFileName, outputFileName);

    /* Code from slides for GSL random number generation */
    const gsl_rng_type *T;
    gsl_rng *r;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc(T);
    gsl_rng_set(r, time(0));

    /* Worst case extra time after the post office doors shut */
    unsigned int buffer = (simParams.maxQueueLength + simParams.numServicePoints)\
                 * simParams.numServicePoints * simParams.lowerLimitServeTime;
    
    unsigned int outputLog[(simParams.closingTime) + buffer][6];

    OUTPUT outputParams;
    /* These values need to be intialized to 0 as they are being incremented
       instead of being assigned a value */
    outputParams.totalcustomersServed = 0;
    outputParams.totalcustomersTimedOut = 0;
    outputParams.totalcustomersUnfulfilled = 0;
    outputParams.totalTimeAfterClose = 0;
    outputParams.totalWaitTime = 0;

    printf("Test1\n");

    /* Run Simulation(s) */
    int i;
    for (i = 0; i < numSims; i++)
        runSim(simParams, outputFileName, *r, outputLog, &outputParams);

    printf("Test2\n");

    for (i = 0; i < outputParams.currentTime; i++)
    {
        if (i == outputParams.closingTime)
            printf("----------- Post Office Closed -----------\n\n");
        printf("Current Time:           %d\n", outputLog[i][0]);
        printf("Customers being Served: %d\n", outputLog[i][1]);
        printf("Customers in Queue:     %d\n", outputLog[i][2]);
        printf("Customers Fulfilled:    %d\n", outputLog[i][3]);
        printf("Customers Unfulfilled:  %d\n", outputLog[i][4]);
        printf("Customers Timed-out:    %d\n", outputLog[i][5]);
        printf("\n");
    }

    /* Write to results output file */
    if (numSims == 1)
    {
        writeLogsOneSim(outputFileName, outputLog, outputParams);
        printf("Simulation ran successfully\n");
    }
    else
    {
        writeLogs(outputFileName, numSims, outputParams);
        printf("%d simulations ran successfully\n", numSims);
    }

    printf("TestFINAL\n");

    gsl_rng_free(r);
    return 0;
}

/*
    Function: runSim
    ---------------------------------------------------------------------------
    Runs one simulation of a post office queue.

    simParams:      a struct that contains all simulation parameters
    outputFileName: the name of the results text file
    r:              Used for random number generation
    outputLog:      a 2D array containing data for each time time interval
                    during the simulation

    returns:        a struct containing data required for the results output
*/
void runSim(INPUT simParams, char outputFileName[], gsl_rng r,
              unsigned int outputLog[][6], OUTPUT *outputParams)
{
    unsigned int currentTime = 0;
    unsigned int customersTotal = 0;
    unsigned int customersServed = 0;
    unsigned int customersTimedOut = 0;
    unsigned int customersUnfulfilled = 0;
    unsigned int customersAtServicePoint = 0;

    unsigned int totalWaitTime = 0;
    SERVICEPOINT servicePoints[simParams.numServicePoints];

    unsigned int busyServicePoints = 0;

    NODE *customerQueue = NULL;

    customerQueue = (NODE *) malloc(sizeof(NODE));
    customerQueue->next = NULL;
    customerQueue->previous = NULL;
    customerQueue->waitLimit = INT_MIN;
    customerQueue->waitCurrent = INT_MIN;

    int i;
    while (customersAtServicePoint > 0 || size(customerQueue) > 0 ||
           currentTime < simParams.closingTime)
    {
        /* Customers leave service point */
        customersServed += fulfillCustomer(simParams.numServicePoints,
                                           servicePoints);

        /* Customers arrive at service point */
        totalWaitTime += startServingCustomer(simParams, servicePoints,
                                              customerQueue, r);

        /* Customer reaches wait limit */
        customersTimedOut += checkWaitLimit(&customerQueue);

        /* New Customers arrive iff the post office is open */
        if (currentTime < simParams.closingTime)
        {
            unsigned int newCustomers = \
            gsl_ran_poisson(&r, simParams.meanNewCustomers);
            for (i = 0; i < newCustomers; i++)
            {
                if (size(customerQueue) < simParams.maxQueueLength || \
                    simParams.maxQueueLength == -1)
                {
                    unsigned int waitLimit = \
                    (int) gsl_ran_flat(&r, simParams.lowerLimitWaitTolerance,
                                       simParams.upperLimitWaitTolerance);
                    enqueue(customerQueue, waitLimit);
                    customersTotal++;
                }
                else {
                    customersUnfulfilled++;
                }
            }
        }

        /* Count the number of customers at a service point */
        customersAtServicePoint = 0;
        for (i = 0; i < simParams.numServicePoints; i++)
        {
            if (servicePoints[i].id == 1)
                customersAtServicePoint++;
        }

        /* Increment the wait time of all customers in the queue and
        service points by 1 */
        updateWait(customerQueue);

        for (i = 0; i < simParams.numServicePoints; i++)
        {
            if (servicePoints[i].id == 1)
                servicePoints[i].timeTaken++;
        }

        /* Save the customer data for this time interval to the 2D array */
        outputLog[currentTime][0] = currentTime;
        outputLog[currentTime][1] = customersAtServicePoint;
        outputLog[currentTime][2] = size(customerQueue);
        outputLog[currentTime][3] = customersServed;
        outputLog[currentTime][4] = customersUnfulfilled;
        outputLog[currentTime][5] = customersTimedOut;

        /* Increment Time interval of this simulation */
        currentTime++;

    }

    outputParams->currentTime = currentTime;
    outputParams->closingTime = simParams.closingTime;
    outputParams->totalWaitTime += totalWaitTime;
    outputParams->totalcustomersServed += customersServed;
    outputParams->totalcustomersUnfulfilled += customersUnfulfilled;
    outputParams->totalcustomersTimedOut += customersTimedOut;
    outputParams->totalTimeAfterClose += currentTime - simParams.closingTime;

    printf("%d\n", outputParams->closingTime);
    printf("%d\n", outputParams->currentTime);


    freeQueue(&customerQueue);
}

/*
    Function: fulfillCustomer
    ---------------------------------------------------------------------------
    Iterates through the service points and checks whether any customers have
    finished being served.

    numServicePoints: the number of service points
    servicePoints:    the array of service points

    returns:          the number of customers that have finished at the post
                      office during this time unit
*/
unsigned int fulfillCustomer(unsigned int numServicePoints, SERVICEPOINT servicePoints[])
{
    unsigned int customersServed = 0;
    int i;
    for (i = 0; i < numServicePoints; i++)
    {
        if (servicePoints[i].timeTaken >= servicePoints[i].timeDone &&
            servicePoints[i].id == 1)
        {
            customersServed++;
            servicePoints[i].id = 0;
        }
    }
    return customersServed;
}

/*
    Function: startServingCustomer
    ---------------------------------------------------------------------------
    Iterates through the service points and checks whether any service points
    are empty. If there are, remove a customer at the front of the queue and
    add them to the empty service point.

    simParams:     a struct that contains all simulation parameters
    servicePoints: the array of service points
    customerQueue: the customer queue linked list

    returns:       the total queue waiting time of all customers that have
                   arrived at a service point during this time unit
*/
unsigned int startServingCustomer(INPUT simParams, SERVICEPOINT servicePoints[],
                         NODE customerQueue[], gsl_rng r)
{
    unsigned int totalWaitTime = 0;
    int i;
    for (i = 0; i < simParams.numServicePoints; i++)
    {
        if (servicePoints[i].id != 1 && size(customerQueue) > 0)
        {
            unsigned int waitTime = dequeue(&customerQueue);
            totalWaitTime += waitTime;
            SERVICEPOINT servicePoint;
            servicePoint.timeTaken = 0;
            unsigned int timeToServe = (int) \
            gsl_ran_rayleigh_tail(&r, simParams.scaleServeTime,
                                  simParams.lowerLimitServeTime);
            servicePoint.timeDone = timeToServe;
            servicePoint.id = 1;
            servicePoints[i] = servicePoint;
        }
    }
    return totalWaitTime;
}
