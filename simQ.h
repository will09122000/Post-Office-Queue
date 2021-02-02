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
void runSim();
int fulfillCustomer(int *numServicePoints, SERVICEPOINT servicePoints[]);
void startServingCustomer(int *numServicePoints, SERVICEPOINT servicePoints[], NODE customerQueue[]);
void newCustomer(NODE customerQueue[], int *customersTotal, int *maxQueueLength);