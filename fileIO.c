#include <fileIO.h>

void getSimParameters(char inputFileName[], int *maxQueueLength)
{
    FILE *fp;
    int numServicePoints;
    int closingTime;
    int meanNewCustomers;
    int stanDevNewCustomers;
    int meanWaitTolerance;
    int standDevWaitTolerance;
    if ( (fp = fopen(inputFileName, "r")) == NULL )
    {
        printf("Unable to open %s\n", inputFileName);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        exit(1);
    }

    fscanf(fp,
           "maxQueueLength: %d\nnumServicePoints: %d\nclosingTime: %d\nmeanNewCustomers: %d\nstanDevNewCustomers: %d\nmeanWaitTolerance: %d\nstandDevWaitTolerance: %d\n",
           &maxQueueLength, &numServicePoints, &closingTime, &meanNewCustomers, &stanDevNewCustomers, &meanWaitTolerance, &standDevWaitTolerance);

    printf("%d\n", &maxQueueLength);
    printf("%d\n", numServicePoints);
    printf("%d\n", closingTime);
    printf("%d\n", meanNewCustomers);
    printf("%d\n", stanDevNewCustomers);
    printf("%d\n", meanWaitTolerance);
    printf("%d\n", standDevWaitTolerance);
    printf("\n");
}