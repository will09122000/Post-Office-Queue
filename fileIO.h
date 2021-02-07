#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/* */
struct simParams
{
    int maxQueueLength;
    int numServicePoints;
    int closingTime;
    int meanNewCustomers;
    int lowerLimitWaitTolerance;
    int upperLimitWaitTolerance;
    int scaleServeTime;
    int lowerLimitServeTime;
};

typedef struct simParams INPUT;

/* A struct that holds data required to produce the output file for both one
   simulation and multiple simulations */
struct outputParams
{
    int currentTime;
    int closingTime;
    int totalWaitTime;
    int totalcustomersServed;
    int totalcustomersUnfulfilled;
    int totalcustomersTimedOut;
    int totalTimeAfterClose;
};

typedef struct outputParams OUTPUT;
