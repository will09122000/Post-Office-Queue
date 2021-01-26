#include <stdlib.h>

typedef struct Customer CUSTOMER;

struct Customer {
    int waitLimit, currentWait;
};

CUSTOMER* createQueue(unsigned maxLength)
{
    CUSTOMER* customer = (CUSTOMER*)malloc(sizeof(CUSTOMER));
    customer->waitLimit = 5;
    customer->currentWait = 0;
    return customer;
}