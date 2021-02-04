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
           &simParams[0],
           &simParams[1],
           &simParams[2],
           &simParams[3],
           &simParams[4],
           &simParams[5],
           &simParams[6],
           &simParams[7]);

/*

    int x;
    for(x=0; x<8; x++)
    {
        simParams[x] = test[x];
    }
    */
}
