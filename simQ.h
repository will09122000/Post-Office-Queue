#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <fileIO.c>
#include <queue.c>

struct servicePoint
{
    unsigned int timeTaken;
    unsigned int timeDone;
    int id;
};
typedef struct servicePoint SERVICEPOINT;

struct outputParams
{
    int currentTime;
    int closingTime;
    int totalWaitTime;
};
typedef struct outputParams OUTPUT;

/* Function Prototypes */
OUTPUT runSim(int simParams[], int numSims, char outputFileName[], gsl_rng r, int outputLog[][6]);

int fulfillCustomer(int *numServicePoints,
                    SERVICEPOINT servicePoints[],
                    int *totalWaitTime);

void startServingCustomer(int *numServicePoints,
                          SERVICEPOINT servicePoints[],
                          NODE customerQueue[],
                          gsl_rng r,
                          int scaleServeTime,
                          int lowerLimitServeTime);
