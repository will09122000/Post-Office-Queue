#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/* A struct that holds all simulation parameters */
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

/* Function prototypes */
void validateInputs(int argc, char **argv);
INPUT getSimParameters(char inputFileName[]);
void writeSimParameters(char inputFileName[], char outputFileName[]);
void writeLogsOneSim(char outputFileName[], unsigned outputLog[][6], OUTPUT outputParams);
void writeLogs(char outputFileName[], unsigned numSims, OUTPUT outputParams);
