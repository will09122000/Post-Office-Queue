#include <fileIO.h>

void getSimParameters(char inputFileName[],
                      int simParams[])
{
    int test[] = {0,0,0,0,0,0,0,0};
    FILE *fp;
    if ( (fp = fopen(inputFileName, "r")) == NULL )
    {
        printf("Unable to open %s\n", inputFileName);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        exit(1);
    }

    fscanf(fp,
           "maxQueueLength: %d\nnumServicePoints: %d\nclosingTime: %d\nmeanNewCustomers: %d\nlowerLimitWaitTolerance: %d\nupperLimitWaitTolerance: %d\nscaleServeTime: %d\nlowerLimitServeTime: %d\n",
           test[0],
           test[1],
           test[2],
           test[3],
           test[4],
           test[5],
           test[6],
           test[7]);

    int x;
    for(x=0; x<8; x++)
    {
        *simParams = test[0];
        simParams++;
    }
}
