#include <simQ.h>

/*
    This file contains the main function as well as functions that handle
    the simulation of a post office queue.
*/

int main (int argc, char **argv)
{
    /* Terminal Arguments */
    char * inputFileName = argv[1];
    int numSims = atoi(argv[2]);
    char * outputFileName = argv[3];

    /* Check the number of simulations is valid */
    if (numSims < 1)
    {
        printf("Number of simulations should be an integer greater than 0 \
(second argument)\n");
        return 1;
    }

    /* Read and Write Simulation Parameters */
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
    int buffer = (simParams.maxQueueLength + simParams.numServicePoints)\
                 * simParams.numServicePoints * simParams.lowerLimitServeTime;
    OUTPUT outputParams;
    int outputLog[(simParams.closingTime) + buffer][6];

    /* Run Simulation(s) */
    int i;
    for (i = 0; i < numSims; i++)
        outputParams = runSim(simParams, numSims, outputFileName, *r, outputLog);

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

    gsl_rng_free(r);
    return 0;
}

/*
    Function: runSim
    ---------------------------------------------------------------------------
    Runs one simulation of a post office queue.

    simParams:      a struct that contains all simulation parameters
    numSims:        the number of simulations to be run
    outputFileName: the name of the results text file
    r:              Used for random number generation
    outputLog:      a 2D array containing data for each time time interval
                    during the simulation

    returns:        a struct containing data required for the results output
*/
OUTPUT runSim(INPUT simParams, int numSims, char outputFileName[], gsl_rng r,
              int outputLog[][6])
{
    unsigned int currentTime;
    int customersTotal = 0;
    int customersServed = 0;
    int customersTimedOut = 0;
    int customersUnfulfilled = 0;
    int customersAtServicePoint = 0;

    int totalWaitTime = 0;
    SERVICEPOINT servicePoints[simParams.numServicePoints];

    int busyServicePoints = 0;

    NODE *customerQueue = NULL;

    customerQueue = (NODE *) malloc(sizeof(NODE));
    customerQueue->next = NULL;
    customerQueue->previous = NULL;
    customerQueue->waitLimit = INT_MIN;
    customerQueue->waitCurrent = INT_MIN;

    OUTPUT outputParams;

    for (currentTime = 0; currentTime < simParams.closingTime; currentTime++)
    {
        /* Customers leave service point */
        customersServed += fulfillCustomer(simParams.numServicePoints,
                                           servicePoints);

        /* Customers arrive at service point */
        totalWaitTime += startServingCustomer(simParams, servicePoints,
                                              customerQueue, r);

        /* Customer reaches wait limit */
        customersTimedOut += checkWaitLimit(&customerQueue);

        /* New Customers */
        unsigned int newCustomers = gsl_ran_poisson(&r, simParams.meanNewCustomers);
        int i;
        for (i = 0; i < newCustomers; i++)
        {
            if (size(customerQueue) < simParams.maxQueueLength || \
                simParams.maxQueueLength == -1)
            {
                int waitLimit = (int) gsl_ran_flat(&r, simParams.lowerLimitWaitTolerance,
                                                   simParams.upperLimitWaitTolerance);
                enqueue(customerQueue, waitLimit);
                customersTotal++;
            }
            else {
                customersUnfulfilled++;
            }
        }

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

        outputLog[currentTime][0] = currentTime;
        outputLog[currentTime][1] = customersAtServicePoint;
        outputLog[currentTime][2] = size(customerQueue);
        outputLog[currentTime][3] = customersServed;
        outputLog[currentTime][4] = customersUnfulfilled;
        outputLog[currentTime][5] = customersTimedOut;

    }

    while (customersAtServicePoint > 0 || size(customerQueue) > 0)
    {
        /* Customers leave service point */
        customersServed += fulfillCustomer(simParams.numServicePoints,
                                           servicePoints);

        /* Customers arrive at service point */
        totalWaitTime += startServingCustomer(simParams, servicePoints,
                                              customerQueue, r);

        /* Customer reaches wait limit */
        customersTimedOut += checkWaitLimit(&customerQueue);

        customersAtServicePoint = 0;
        int i;
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

        outputLog[currentTime][0] = currentTime;
        outputLog[currentTime][1] = customersAtServicePoint;
        outputLog[currentTime][2] = size(customerQueue);
        outputLog[currentTime][3] = customersServed;
        outputLog[currentTime][4] = customersUnfulfilled;
        outputLog[currentTime][5] = customersTimedOut;

        /* Increment Time interval as this loops after the post office has
           closed */
        currentTime++;
    }

    outputParams.currentTime = currentTime;
    outputParams.closingTime = simParams.closingTime;
    outputParams.totalWaitTime += totalWaitTime;
    outputParams.totalcustomersServed += customersServed;
    outputParams.totalcustomersUnfulfilled += customersUnfulfilled;
    outputParams.totalcustomersTimedOut += customersTimedOut;
    outputParams.totalTimeAfterClose += currentTime - simParams.closingTime;

    freeQueue(&customerQueue);
    return outputParams;
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
int fulfillCustomer(int numServicePoints, SERVICEPOINT servicePoints[])
{
    int customersServed = 0;
    int i;
    for (i = 0; i < numServicePoints; i++)
    {
        if (servicePoints[i].timeTaken == servicePoints[i].timeDone &&
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
int startServingCustomer(INPUT simParams, SERVICEPOINT servicePoints[],
                         NODE customerQueue[], gsl_rng r)
{
    int totalWaitTime = 0;
    int i;
    for (i = 0; i < simParams.numServicePoints; i++)
    {
        if (servicePoints[i].id != 1 && size(customerQueue) > 0)
        {
            int waitTime = dequeue(&customerQueue);
            totalWaitTime += waitTime;
            SERVICEPOINT servicePoint;
            servicePoint.timeTaken = 0;
            int timeToServe = (int) gsl_ran_rayleigh_tail(&r, simParams.scaleServeTime,
                                                          simParams.lowerLimitServeTime);
            servicePoint.timeDone = timeToServe;
            servicePoint.id = 1;
            servicePoints[i] = servicePoint;
        }
    }
    return totalWaitTime;
}
