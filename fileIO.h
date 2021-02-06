#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

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