#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <fileIO.c>
#include <queue.c>

/* A struct that represents a single service point in the post office */
struct servicePoint
{
    unsigned int timeTaken;
    unsigned int timeDone;
    int id;
};

typedef struct servicePoint SERVICEPOINT;

/* Function prototypes */
OUTPUT runSim(INPUT simParams, int numSims, char outputFileName[], gsl_rng r,
              int outputLog[][6]);

int fulfillCustomer(int numServicePoints, SERVICEPOINT servicePoints[]);

int startServingCustomer(INPUT simParams, SERVICEPOINT servicePoints[],
                         NODE customerQueue[], gsl_rng r);
