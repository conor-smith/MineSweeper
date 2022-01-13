#include <sys/time.h>
#include <stdlib.h>

long getCurrentTime() {
    struct timeval tVal;

    gettimeofday(&tVal, NULL);

    return (tVal.tv_sec * 1000) + (tVal.tv_usec / 1000);
}