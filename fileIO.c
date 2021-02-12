#include <fileIO.h>

/*
    This file contains functions that handles the reading and writing to input
    parameter and output text files.
*/

/*
    Function: validateInputs
    ---------------------------------------------------------------------------
    Checks all user inputs to the program are valid.

    argc:    number of command-line arguments passed by the user
    argv:    array of character pointers listing all the arguments

    returns: null
*/
void validateInputs(int argc, char **argv)
{
    int error = 0;

    /* Check the number of parameters is valid */
    if (argc != 4)
    {
        printf("Invalid number of arguments, please ensure you have \
[inputFileText.txt numberOfSimulations inputFileText.txt] in that order.\n");
        error = 1;
    }
    
    /* Check the number of simulations is valid */
    if (atoi(argv[2]) < 1)
    {
        printf("Number of simulations (second argument) should be an integer \
greater than 0 and less than or equal to 2147483647. \n");
        error = 1;
    }

    /* Check both the input and output files can be read */
    FILE *fp;
    if ( (fp = fopen(argv[1], "r")) == NULL )
    {
        printf("Unable to open %s\n", argv[1]);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        error = 1;
    }
    else
        fclose(fp);
    if ( (fp = fopen(argv[3], "r")) == NULL )
    {
        printf("Unable to open %s\n", argv[3]);
        fprintf(stderr, "error %d: %s\n", errno, strerror(errno));
        error = 1;
    }
    else
        close(fp);

    if (error == 1)
        exit(1);
}

/*
    Function: getSimParameters
    ---------------------------------------------------------------------------
    Reads and validates the simulation parameters from the text file.

    inputFileName: the name of the parameters text file

    returns:       simulations parameters struct
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

    /* Read the file making sure it's in the correct format */
    INPUT simParams;
    if (fscanf(fp,
           "maxQueueLength: %d\nnumServicePoints: %d\nclosingTime: %d\n\
meanNewCustomers: %d\nlowerLimitWaitTolerance: %d\nupperLimitWaitTolerance: %d\n\
scaleServeTime: %d\nlowerLimitServeTime: %d\n",
           &simParams.maxQueueLength, &simParams.numServicePoints,
           &simParams.closingTime, &simParams.meanNewCustomers,
           &simParams.lowerLimitWaitTolerance, &simParams.upperLimitWaitTolerance,
           &simParams.scaleServeTime, &simParams.lowerLimitServeTime))
        ;
    else
    {
        printf("Invalid input file format, it should look like this:\n\
maxQueueLength: 8\n\
numServicePoints: 3\n\
closingTime: 30\n\
meanNewCustomers: 1\n\
lowerLimitWaitTolerance: 3\n\
upperLimitWaitTolerance: 7\n\
scaleServeTime: 2\n\
lowerLimitServeTime: 3\n");
        exit(1);
    }

    /* Check all simulation parameters are valid */
    if (simParams.maxQueueLength == 0 || simParams.maxQueueLength < -1)
    {
        printf("Invalid maxQueueLength parameter, it should be equal to -1 or \
an integer greater than 0 and less than or equal to 2147483647.\n");
        exit(1);
    }
    if (simParams.numServicePoints < 1 || simParams.closingTime < 1 ||
        simParams.meanNewCustomers < 1 || simParams.lowerLimitWaitTolerance < 1 ||
        simParams.upperLimitWaitTolerance < 1 || simParams.scaleServeTime < 1 ||
        simParams.lowerLimitServeTime < 1)
    {
        printf("Invalid parameters, all parameters except maxQueueLength should \
be an integer greater than 0 and less than or equal to 2147483647.\n");
        exit(1);
    }

    fclose(fp);
    return simParams;
}

/*
    Function: writeSimParameters
    ---------------------------------------------------------------------------
    Writes the simulation parameters to the results text file.

    inputFileName:  the name of the parameters text file
    outputFileName: the name of the results text file

    returns:        null
*/
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

/*
    Function: writeLogsOneSim
    ---------------------------------------------------------------------------
    Writes the simulation results to the results text file if only one
    simulation is run.

    outputFileName: the name of the results text file
    outputLog:      a 2D array containing data for each time time interval
                    during the simulation
    outputParams:   a struct containing data required for the results output

    returns:        null
*/
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

    printf("%d\n", outputParams.totalWaitTime);
    printf("%d\n", outputLog[outputParams.currentTime-1][3]);

    fprintf(fp, "Average Customer Waiting Time: %f\n",
            (float) outputParams.totalWaitTime / 
            (float) outputLog[outputParams.currentTime-1][3]);
}

/*
    Function: writeLogs
    ---------------------------------------------------------------------------
    Writes the simulation results to the results text file if more than one
    simulation is run.

    outputFileName: the name of the results text file
    numSims:        the number of simulations that have been run
    outputParams:   a struct containing data required for the results output

    returns:        null
*/
void writeLogs(char outputFileName[], int numSims, OUTPUT outputParams)
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
