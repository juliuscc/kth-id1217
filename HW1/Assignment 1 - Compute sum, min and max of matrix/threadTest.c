#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int x;
    int y;
} coordinate;

void *test(void *arg)
{
    coordinate *cord = malloc(sizeof(coordinate));
    cord->x = 10;
    cord->y = 0;

    pthread_exit((void *)cord);
}

int main(int argc, char *argv[])
{
    /* init/start thread */
    pthread_t testThread;
    pthread_create(&testThread, NULL, test, NULL);

    /* join thread */
    coordinate *cord;
    pthread_join(testThread, (void **)&cord);

    printf("x is: %d\n", cord->x);
    free(cord);
    return 0;
}