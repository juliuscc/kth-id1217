/* matrix summation using pthreads

	features:
		uses a barrier; the Worker[0] computes
		the total sum from partial sums computed by Workers
		and prints the total sum to the standard output

	usage under Linux:
		gcc matrixSum.c -lpthread
		a.out size numWorkers

*/
#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 10000 /* maximum matrix size */
#define MAXWORKERS 10 /* maximum number of workers */

pthread_mutex_t barrier; /* mutex lock for the barrier */
pthread_cond_t go;		 /* condition variable for leaving */
int numWorkers;			 /* number of workers */

typedef struct
{
	int total;
	int max_i;
	int max_j;
	int min_i;
	int min_j;
} result;

/* timer */
double read_timer()
{
	static bool initialized = false;
	static struct timeval start;
	struct timeval end;
	if (!initialized)
	{
		gettimeofday(&start, NULL);
		initialized = true;
	}
	gettimeofday(&end, NULL);
	return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

double start_time, end_time;  /* start and end times */
int size, stripSize;		  /* assume size is multiple of numWorkers */
int matrix[MAXSIZE][MAXSIZE]; /* matrix */

void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
	int i, j;
	long l; /* use long in case of a 64-bit system */
	pthread_attr_t attr;
	pthread_t workerid[MAXWORKERS];

	/* set global thread attributes */
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	/* initialize mutex and condition variable */
	pthread_mutex_init(&barrier, NULL);
	pthread_cond_init(&go, NULL);

	/* read command line args if any */
	size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
	numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
	if (size > MAXSIZE)
		size = MAXSIZE;
	if (numWorkers > MAXWORKERS)
		numWorkers = MAXWORKERS;
	stripSize = size / numWorkers;

	/* initialize the matrix */
	srand(time(NULL));
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix[i][j] = rand() % 99;
		}
	}

		/* print the matrix */
#ifdef DEBUG
	for (i = 0; i < size; i++)
	{
		printf("[ ");
		for (j = 0; j < size; j++)
		{
			printf(" %d", matrix[i][j]);
		}
		printf(" ]\n");
	}
#endif

	/* do the parallel work: create the workers */
	start_time = read_timer();
	result mainResult = {
		.total = 0,
		.max_i = 0,
		.max_j = 0,
		.min_i = 0,
		.min_j = 0};
	result *res;
	for (l = 0; l < numWorkers; l++)
		pthread_create(&workerid[l], &attr, Worker, (void *)l);
	for (l = 0; l < numWorkers; l++)
	{
		pthread_join(workerid[l], (void **)&res);
		mainResult.total += res->total;
		if (matrix[res->max_i][res->max_j] > matrix[mainResult.max_i][mainResult.max_j])
		{
			mainResult.max_i = res->max_i;
			mainResult.max_j = res->max_j;
		}
		if (matrix[res->min_i][res->min_j] < matrix[mainResult.min_i][mainResult.min_j])
		{
			mainResult.min_i = res->min_i;
			mainResult.min_j = res->min_j;
		}
		free(res);
	}

	/* get end time */
	end_time = read_timer();

	/* print results */
	int total = mainResult.total;
	int max = matrix[mainResult.max_i][mainResult.max_j];
	int min = matrix[mainResult.min_i][mainResult.min_j];

#ifndef STRIPPEDPRINT
	printf("The total is %d\n", total);
	printf("The max element is %d, at position: (%d;%d)\n", max, mainResult.max_i + 1, mainResult.max_j + 1);
	printf("The min element is %d, at position: (%d;%d)\n", min, mainResult.min_i + 1, mainResult.min_j + 1);
	printf("The execution time is %g sec\n", end_time - start_time);
#endif
#ifdef STRIPPEDPRINT
	printf("%g\n", end_time - start_time);
#endif

	pthread_exit(NULL);
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
void *Worker(void *arg)
{
	long myid = (long)arg;
	int i, j, first, last;
#ifdef DEBUG
	printf("worker %d (pthread id %d) has started\n", myid, pthread_self());
#endif

	/* determine first and last rows of my strip */
	first = myid * stripSize;
	last = (myid == numWorkers - 1) ? (size - 1) : (first + stripSize - 1);

	/* sum values in my strip */
	result *res = malloc(sizeof(result));

	res->total = 0;
	res->max_i = first;
	res->max_j = 0;
	res->min_i = first;
	res->min_j = 0;

	int max = matrix[first][0];
	int min = matrix[first][0];

	for (i = first; i <= last; i++)
	{
		for (j = 0; j < size; j++)
		{
			res->total += matrix[i][j];
			if (max < matrix[i][j])
			{
				res->max_i = i;
				res->max_j = j;
				max = matrix[i][j];
			}
			if (min > matrix[i][j])
			{
				res->min_i = i;
				res->min_j = j;
				min = matrix[i][j];
			}
		}
	}

	pthread_exit((void *)res);
}
