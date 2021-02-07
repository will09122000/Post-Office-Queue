#include <fileIO.h>

/*
    This file contains functions that handles the reading and writing to input
    parameter and output text files.
*/

/*
    Function: getSimParameters
    --------------------
    Reads the simulation parameters from the text file

    inputFileName: the name of the text file

    returns: simulations parameters struct
*/
INPUT getSimParameters(char inputFileName[])
{
    FILE *fp;
    if ( (fp = fopen(inputFileName, "r")) == NULL )
    {
        printf("Unable to open %s\n", inputFileName);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        exit(1);
    }

    INPUT simParams;
    fscanf(fp,
           "maxQueueLength: %d\nnumServicePoints: %d\nclosingTime: %d\n\
meanNewCustomers: %d\nlowerLimitWaitTolerance: %d\nupperLimitWaitTolerance: %d\n\
scaleServeTime: %d\nlowerLimitServeTime: %d\n",
           &simParams.maxQueueLength, &simParams.numServicePoints, &simParams.closingTime,
           &simParams.meanNewCustomers, &simParams.lowerLimitWaitTolerance,
           &simParams.upperLimitWaitTolerance, &simParams.scaleServeTime,
           &simParams.lowerLimitServeTime);

    fclose(fp);
    return simParams;
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
    while(fgets(content, sizeof(content), fpIn) != NULL)
        fprintf(fpOut, "%s", content);

    fputs("\n", fpOut);

    fclose(fpIn);
    fclose(fpOut);
}

void writeLogsOneSim(char outputFileName[], int outputLog[][6],
                     OUTPUT outputParams)
{
    FILE *fp;
    if ( (fp = fopen(outputFileName, "a")) == NULL )
    {
        printf("Unable to open %s\n", outputFileName);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        exit(1);
    }
    int i;
    for (i = 0; i < outputParams.currentTime; i++)
    {
        if (i == outputParams.closingTime)
            fputs("----------- Post Office Closed -----------\n\n", fp);
        fprintf(fp, "Current Time:           %d\n", outputLog[i][0]);
        fprintf(fp, "Customers being Served: %d\n", outputLog[i][1]);
        fprintf(fp, "Customers in Queue:     %d\n", outputLog[i][2]);
        fprintf(fp, "Customers Fulfilled:    %d\n", outputLog[i][3]);
        fprintf(fp, "Customers Unfulfilled:  %d\n", outputLog[i][4]);
        fprintf(fp, "Customers Timed-out:    %d\n", outputLog[i][5]);
        fputs("\n", fp);
    }
    fprintf(fp, "Time taken to serve remaining customers after the post \
office has closed: %d\n", (outputParams.currentTime-outputParams.closingTime));

    fprintf(fp, "Average Customer Waiting Time: %f\n",
            (float) outputParams.totalWaitTime / 
            (float) outputLog[outputParams.currentTime-1][3]);
}

void writeLogs(char outputFileName[], int numSims, int buffer, OUTPUT outputParams)
{
    FILE *fp;
    if ( (fp = fopen(outputFileName, "a")) == NULL )
    {
        printf("Unable to open %s\n", outputFileName);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        exit(1);
    }

    fprintf(fp, "Average Number of Fulfilled Customers:      %f\n",
            (float) outputParams.totalcustomersServed / (float) numSims);

    fprintf(fp, "Average Number of Unfulfilled Customers:    %f\n",
            (float) outputParams.totalcustomersUnfulfilled / (float) numSims);

    fprintf(fp, "Average Number of Timed-out Customers:      %f\n",
            (float) outputParams.totalcustomersTimedOut / (float) numSims);

    fprintf(fp, "Average time taken to serve remaining\ncustomers \
after the post office has closed: %f\n",
            (float) outputParams.totalTimeAfterClose / (float) numSims);

    fprintf(fp, "Average Customer Waiting Time:              %f\n",
            (float) outputParams.totalWaitTime /
            (float) outputParams.totalcustomersServed);

    fclose(fp);
}