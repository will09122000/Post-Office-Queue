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

/* Function Prototypes */
void runSim(int simParams[], int numSims, char outputFileName[], gsl_rng r);

int fulfillCustomer(int *numServicePoints,
                    SERVICEPOINT servicePoints[],
                    int *totalWaitTime);

void startServingCustomer(int *numServicePoints,
                          SERVICEPOINT servicePoints[],
                          NODE customerQueue[],
                          gsl_rng r,
                          int scaleServeTime,
                          int lowerLimitServeTime);
