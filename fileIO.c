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

void writeLogsOneSim(char outputFileName[], int outputLog[][6], int currentTime, int closingTime, int totalWaitTime)
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
    fprintf(fp, "Time taken to serve remaining customers after the post office has closed: %d\n", (currentTime-closingTime));
    fprintf(fp, "Average Customer Waiting Time: %f\n", (float) totalWaitTime / (float) outputLog[currentTime-1][3]);
}

void writeLogs(char outputFileName[], int *avgOutputLog, int currentTime, int closingTime, int totalwaitTime, int numSims, int buffer)
{
    int totalcustomersServed = 0;
    int totalcustomersUnfulfilled = 0;
    int totalcustomersTimedOut = 0;
    int avgOutputLogTest[numSims][closingTime + buffer][6];
    int i, j, k;
    for(i=0;i<numSims;i++)
	{
		for(j=0;j<closingTime + buffer;j++)
		{
			for(k=0;k<6;k++)
                avgOutputLogTest[i][j][k] = avgOutputLog[(i * ((closingTime + buffer)*6)) + (j * 6) + k];
		}
	}
    for(i=0;i<numSims;i++)
	{
		for(j=0;j<closingTime + buffer;j++)
		{
            printf("%d\n", totalcustomersServed);
            totalcustomersServed += avgOutputLogTest[i][j][3];
            totalcustomersUnfulfilled += avgOutputLogTest[i][j][4];
            totalcustomersTimedOut += avgOutputLogTest[i][j][5];
		}
	}

    FILE *fp;
    if ( (fp = fopen(outputFileName, "a")) == NULL )
    {
        printf("Unable to open %s\n", outputFileName);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        exit(1);
    }

    fprintf(fp, "Average Number of Fulfilled Customers:   %d\n", totalcustomersServed / numSims);
    fprintf(fp, "Average Number of Unfulfilled Customers: %d\n", totalcustomersUnfulfilled / numSims);
    fprintf(fp, "Average Number of Timed-out Customers:   %d\n", totalcustomersTimedOut / numSims);

    /*
    fprintf(fp, "Time taken to serve remaining customers after the post office has closed: %d\n", (currentTime-closingTime));
    fprintf(fp, "Average Customer Waiting Time: %f\n", (float) totalWaitTime / (float) outputLog[currentTime-1][3]);
    */

}