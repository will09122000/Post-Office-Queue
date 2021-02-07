#include <simQ.h>

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

    /* Worst case extra time after the post office closes
       (Max queue length + Number of service points) *
       Longest possible time at service point */
    int buffer = (simParams.maxQueueLength + simParams.numServicePoints)\
                 * simParams.numServicePoints * simParams.lowerLimitServeTime;
    OUTPUT outputParams;
    int outputLog[(simParams.closingTime) + buffer][6];

    /* Run Simulation(s) */
    int i;
    for (i = 0; i < numSims; i++)
        outputParams = runSim(simParams, numSims, outputFileName, *r, outputLog);

    if (numSims == 1)
        writeLogsOneSim(outputFileName, outputLog, outputParams);
    else
        writeLogs(outputFileName, numSims, buffer, outputParams);

    return 0;
}

OUTPUT runSim(INPUT simParams, int numSims, char outputFileName[], gsl_rng r,
              int outputLog[][6])
{
    unsigned int currentTime;
    int customersTotal = 0;
    int customersServed = 0;
    int customersTimedOut = 0;
    int customersUnfulfilled = 0;
    int customersAtServicePoint = 0;

    /* Simulation parameters separated into individual variables for improved
       readability 
    int maxQueueLength = simParams[0];
    int numServicePoints = simParams[1];
    int closingTime = simParams[2];
    int meanNewCustomers = simParams[3];
    int lowerLimitWaitTolerance = simParams[4];
    int upperLimitWaitTolerance = simParams[5];
    int scaleServeTime = simParams[6];
    int lowerLimitServeTime = simParams[7];
    */

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
        customersServed += fulfillCustomer(simParams.numServicePoints, servicePoints,
                                           &totalWaitTime);

        /* Customers arrive at service point */
        startServingCustomer(simParams.numServicePoints, servicePoints, customerQueue,
                             r, simParams.scaleServeTime, simParams.lowerLimitServeTime);

        /* Customer reaches wait limit */
        customersTimedOut += checkWaitLimit(&customerQueue);

        /* New Customers */
        unsigned int newCustomers = gsl_ran_poisson(&r, simParams.meanNewCustomers);
        int i;
        for (i = 0; i < newCustomers; i++)
        {
            if (size(customerQueue) < simParams.maxQueueLength || simParams.maxQueueLength == -1)
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
        customersServed += fulfillCustomer(simParams.numServicePoints, servicePoints,
                                           &totalWaitTime);

        /* Customers arrive at service point */
        startServingCustomer(simParams.numServicePoints, servicePoints, customerQueue,
                             r, scaleServeTime, simParams.lowerLimitServeTime);

        /* Customer reaches wait limit */
        customersTimedOut += checkWaitLimit(&customerQueue);

        customersAtServicePoint = 0;
        int i;
        for (i = 0; i < numServicePoints; i++)
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

    free(customerQueue);
    return outputParams;
}

int fulfillCustomer(int numServicePoints, SERVICEPOINT servicePoints[],
                    int *totalWaitTime)
{
    int customersServed = 0;
    int i;
    for (i = 0; i < numServicePoints; i++)
    {
        if (servicePoints[i].timeTaken == servicePoints[i].timeDone &&
            servicePoints[i].id == 1)
        {
            customersServed++;
            *totalWaitTime += servicePoints[i].timeTaken;
            servicePoints[i].id = 0;
        }
    }
    return customersServed;
}

void startServingCustomer(int numServicePoints, SERVICEPOINT servicePoints[],
                          NODE customerQueue[], gsl_rng r,  int scaleServeTime,
                          int lowerLimitServeTime)
{
    int i;
    for (i = 0; i < numServicePoints; i++)
    {
        if (servicePoints[i].id != 1)
        {
            NODE * customer = dequeue(&customerQueue);
            if (customer) {
                SERVICEPOINT servicePoint;
                servicePoint.timeTaken = 0;
                int timeToServe = (int) gsl_ran_rayleigh_tail(&r, scaleServeTime,
                                                              lowerLimitServeTime);
                servicePoint.timeDone = timeToServe;
                servicePoint.id = 1;
                servicePoints[i] = servicePoint;
            }
        }
    }
}
