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
    printf("%d\n", &simParams[0]);
    printf("%d\n", &simParams[1]);
    printf("%d\n", &simParams[2]);
    printf("%d\n", &simParams[3]);
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
        fprintf(fpOut, "%s", content);

    fputs("\n", fpOut);

    fclose(fpIn);
    fclose(fpOut);
}

void writeLogsOneSim(char outputFileName[], int outputLog[][6], int currentTime,
                     int closingTime, int totalWaitTime)
{
    FILE *fp;
    if ( (fp = fopen(outputFileName, "a")) == NULL )
    {
        printf("Unable to open %s\n", outputFileName);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        exit(1);
    }
    int i;
    for (i=0; i < currentTime; i++)
    {
        if (i == closingTime)
            fputs("----------- Post Office Closed -----------\n\n", fp);
        fprintf(fp, "Current Time:           %d\n", outputLog[i][0]);
        fprintf(fp, "Customers being Served: %d\n", outputLog[i][1]);
        fprintf(fp, "Customers in Queue:     %d\n", outputLog[i][2]);
        fprintf(fp, "Customers Fulfilled:    %d\n", outputLog[i][3]);
        fprintf(fp, "Customers Unfulfilled:  %d\n", outputLog[i][4]);
        fprintf(fp, "Customers Timed-out:    %d\n", outputLog[i][5]);
        fputs("\n", fp);
    }
    fprintf(fp, "Time taken to serve remaining customers after the post office\
            has closed: %d\n", (currentTime-closingTime));
    fprintf(fp, "Average Customer Waiting Time: %f\n",
            (float) totalWaitTime / (float) outputLog[currentTime-1][3]);
}

void writeLogs(char outputFileName[], int *avgOutputLog, int closingTime,
               int numSims, int buffer, int currentTime[], int waitTime[])
{
    int totalcustomersServed = 0;
    int totalcustomersUnfulfilled = 0;
    int totalcustomersTimedOut = 0;
    int totalTimeAfterClose = 0;
    int totalWaitTime = 0;

    int i;
    for(i=0; i<numSims; i++)
    {
        int customersServedIndex = (i * (closingTime + buffer) * 6) +\
                                   (currentTime[i]-1 * 6) + 3;
        int customersUnfulfilledIndex = (i * (closingTime + buffer) * 6) +\
                                        (currentTime[i]-1 * 6) + 4;
        int customersTimedOutIndex = (i * (closingTime + buffer) * 6) +\
                                     (currentTime[i]-1 * 6) + 5;
        totalcustomersServed += avgOutputLog[customersServedIndex];
        totalcustomersUnfulfilled += avgOutputLog[customersUnfulfilledIndex];
        totalcustomersTimedOut += avgOutputLog[customersTimedOutIndex];
        totalTimeAfterClose += (currentTime[i]-1) - closingTime;
        totalWaitTime += waitTime[i];
    }

    FILE *fp;
    if ( (fp = fopen(outputFileName, "a")) == NULL )
    {
        printf("Unable to open %s\n", outputFileName);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        exit(1);
    }

    fprintf(fp, "Average Number of Fulfilled Customers:      %f\n",
            (float) totalcustomersServed / (float) numSims);
    fprintf(fp, "Average Number of Unfulfilled Customers:    %f\n",
            (float) totalcustomersUnfulfilled / (float) numSims);
    fprintf(fp, "Average Number of Timed-out Customers:      %f\n",
            (float) totalcustomersTimedOut / (float) numSims);
    fprintf(fp, "Average time taken to serve remaining\ncustomers\
            after the post office has closed: %f\n",
            (float) totalTimeAfterClose / (float) numSims);
    fprintf(fp, "Average Customer Waiting Time:              %f\n",
            (float) totalWaitTime / (float) totalcustomersServed);

    fclose(fp);
}