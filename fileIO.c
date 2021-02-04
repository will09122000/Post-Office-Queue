#include <fileIO.h>

void getSimParameters(char inputFileName[], int simParams[])
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
           &simParams[0],
           &simParams[1],
           &simParams[2],
           &simParams[3],
           &simParams[4],
           &simParams[5],
           &simParams[6],
           &simParams[7]);

    fclose(fp);
}

void writeSimParameters(char inputFileName[], char outputFileName[])
{
    char content[100];

    FILE *fpIn;
    if ( (fpIn = fopen(inputFileName, "r")) == NULL )
    {
        printf("Unable to open %s\n", inputFileName);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        exit(1);
    }

    FILE *fpOut;
    if ( (fpOut = fopen(outputFileName, "w")) == NULL )
    {
        printf("Unable to open %s\n", outputFileName);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        exit(1);
    }

    fputs("# Input Parameters\n", fpOut);
    while(fgets(content, sizeof(content), fpIn) !=NULL)
    {
        fprintf(fpOut, "%s", content);
    }
    fputs("\n", fpOut);
    fclose(fpIn);



    fclose(fpOut);
}

void writeLogs(char outputFileName[], int outputLog[], int currentTime, int closingTime, int totalWaitTime)
{
    FILE *fp;
    if ( (fp = fopen(outputFileName, "a")) == NULL )
    {
        printf("Unable to open %s\n", outputFileName);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        exit(1);
    }
    int i, j;
    for (i=0; i < currentTime*6; i+=6)
    {
        fprintf(fp, "Current Time: %d\n", outputLog[i]);
        fprintf(fp, "Customers being Served: %d\n", outputLog[i+1]);
        fprintf(fp, "Customers in Queue: %d\n", outputLog[i+2]);
        fprintf(fp, "Customers Fulfilled: %d\n", outputLog[i+3]);
        fprintf(fp, "Customers Unfulfilled: %d\n", outputLog[i+4]);
        fprintf(fp, "Customers Timed-out: %d\n", outputLog[i+5]);
        if (i/6 == closingTime)
            fputs("\n----------- Post Office Closed -----------\n", fp);
        fputs("\n", fp);
    }
    fprintf(fp, "Time taken from closing time until all remaining customers have been served: %d\n", (currentTime-closingTime)-1);
    float avgWaitTime = totalWaitTime / outputLog[(currentTime*6)-3];
    fprintf(fp, "Average Customer Waiting Time: %f\n", avgWaitTime);
}