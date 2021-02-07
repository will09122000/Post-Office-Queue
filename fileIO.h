#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/* A struct that holds data required to produce the output file for both one
   simulation and multiple simulations */
struct outputParams
{
    int currentTime;
    int closingTime;
    int totalWaitTime;
    int totalcustomersServed;
    int totalcustomersUnfulfilled;
    int totalcustomersTimedOut;
    int totalTimeAfterClose;
};

typedef struct outputParams OUTPUT;
