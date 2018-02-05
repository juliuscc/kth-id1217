/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
	 gcc -O -fopenmp -o matrixSum-openmp matrixSum-openmp.c 
	 ./matrixSum-openmp size numWorkers

*/
#include <stdlib.h>
#include <omp.h>

double start_time, end_time;

#include <stdio.h>
#define MAXSIZE 10000 /* maximum matrix size */
#define MAXWORKERS 24 /* maximum number of workers */

int numWorkers;
int size;
int matrix[MAXSIZE][MAXSIZE];
void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
	int i, j, total = 0;

	/* read command line args if any */
	size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
	numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
	if (size > MAXSIZE)
		size = MAXSIZE;
	if (numWorkers > MAXWORKERS)
		numWorkers = MAXWORKERS;

	omp_set_num_threads(numWorkers);

	/* initialize the matrix */
	for (i = 0; i < size; i++)
	{
		//  printf("[ ");
		for (j = 0; j < size; j++)
		{
			matrix[i][j] = rand() % 99;
			//	  printf(" %d", matrix[i][j]);
		}
		//	  printf(" ]\n");
	}

	int min_val = matrix[0][0];
	int max_val = matrix[0][0];

	start_time = omp_get_wtime();
#pragma omp parallel for reduction(+:total), reduction(max:max_val), reduction(min:min_val) private(j)
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
		{
			total += matrix[i][j];
			if (matrix[i][j] > max_val)
			{
				max_val = matrix[i][j];
			}
			if (matrix[i][j] < min_val)
			{
				min_val = matrix[i][j];
			}
		}
	// implicit barrier

	end_time = omp_get_wtime();

	printf("the total is %d\n", total);
	printf("the maximum element is: %d on coordinates: (%d;%d)", max_val, 0, 0);
	printf("the minimum element is: %d on coordinates: (%d;%d)", min_val, 0, 0);
	printf("it took %g seconds\n", end_time - start_time);
}
