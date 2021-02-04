#include <fileIO.h>

void getSimParameters(char inputFileName[],
                      int *maxQueueLength,
                      int *numServicePoints,
                      int *closingTime,
                      int *meanNewCustomers,
                      int *lowerLimitWaitTolerance,
                      int *upperLimitWaitTolerance,
                      int *scaleServeTime,
                      int *lowerLimitServeTime)
{
    FILE *fp;
    if ( (fp = fopen(inputFileName, "r")) == NULL )
    {
        printf("Unable to open %s\n", inputFileName);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        exit(1);
    }

    fscanf(fp,
           "maxQueueLength: %d\nnumServicePoints: %d\nclosingTime: %d\nmeanNewCustomers: %d\nlowerLimitWaitTolerance: %d\nupperLimitWaitTolerance: %d\nscaleServeTime: %d\nlowerLimitServeTime: %d\n",
           maxQueueLength,
           numServicePoints,
           closingTime,
           meanNewCustomers,
           lowerLimitWaitTolerance,
           upperLimitWaitTolerance,
           scaleServeTime,
           lowerLimitServeTime);
}
