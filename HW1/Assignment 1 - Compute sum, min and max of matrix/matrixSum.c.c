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
int numArrived = 0;		 /* number who have arrived */

struct element
{
	int i_pos;
	int j_pos;
	int val;
};

/* a reusable counter barrier */
void Barrier()
{
	pthread_mutex_lock(&barrier);
	numArrived++;
	if (numArrived == numWorkers)
	{
		numArrived = 0;
		pthread_cond_broadcast(&go);
	}
	else
		pthread_cond_wait(&go, &barrier);
	pthread_mutex_unlock(&barrier);
}

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

double start_time, end_time;	 /* start and end times */
int size, stripSize;			 /* assume size is multiple of numWorkers */
int sums[MAXWORKERS];			 /* partial sums */
int matrix[MAXSIZE][MAXSIZE];	/* matrix */
struct element max_arr[MAXSIZE]; /* every rows max element */
struct element min_arr[MAXSIZE]; /* every rows min element */
int nextRow = 0;				 /* For bag of tasks */

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
		max_arr[i].i_pos = i;
		max_arr[i].j_pos = 0;
		max_arr[i].val = matrix[i][0];
		min_arr[i].i_pos = i;
		min_arr[i].j_pos = 0;
		min_arr[i].val = matrix[i][0];
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
	for (l = 0; l < numWorkers; l++)
		pthread_create(&workerid[l], &attr, Worker, (void *)l);
	for (l = 0; l < numWorkers; l++)
		pthread_join(workerid[l], NULL);

	int total = 0;
	struct element max = max_arr[0];
	struct element min = min_arr[0];
	for (i = 0; i < size; i++)
	{
		total += sums[i];
		if (max.val < max_arr[i].val)
		{
			max.i_pos = max_arr[i].i_pos;
			max.j_pos = max_arr[i].j_pos;
			max.val = max_arr[i].val;
		}
		if (min.val > min_arr[i].val)
		{
			min.i_pos = min_arr[i].i_pos;
			min.j_pos = min_arr[i].j_pos;
			min.val = min_arr[i].val;
		}
	}
	/* get end time */
	end_time = read_timer();
	/* print results */
	printf("The total is %d\n", total);
	printf("The max element is %d, at position: (%d;%d)\n", max.val, max.j_pos + 1, max.i_pos + 1);
	printf("The min element is %d, at position: (%d;%d)\n", min.val, min.j_pos + 1, min.i_pos + 1);
	printf("The execution time is %g sec\n", end_time - start_time);

	pthread_exit(NULL);
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
void *Worker(void *arg)
{
	long myid = (long)arg;
	int total, j;

	int row;
	while (1)
	{
		row = __sync_fetch_and_add(&nextRow, 1);
		if (row >= size)
			pthread_exit(NULL);

		/* sum values in my strip */
		total = 0;
		struct element max = {.i_pos = row, .j_pos = 0, .val = matrix[row][0]};
		struct element min = {.i_pos = row, .j_pos = 0, .val = matrix[row][0]};
		for (j = 0; j < size; j++)
		{
			total += matrix[row][j];
			if (max.val < matrix[row][j])
			{
				max.i_pos = row;
				max.j_pos = j;
				max.val = matrix[row][j];
			}
			if (min.val > matrix[row][j])
			{
				min.i_pos = row;
				min.j_pos = j;
				min.val = matrix[row][j];
			}
		}
		sums[row] = total;
		max_arr[row] = max;
		min_arr[row] = min;
	}
}
