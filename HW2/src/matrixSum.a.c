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

	int max_i = 0;
	int max_j = 0;
	int min_i = 0;
	int min_j = 0;
	int max_i_shared = 0;
	int max_j_shared = 0;
	int min_i_shared = 0;
	int min_j_shared = 0;

	start_time = omp_get_wtime();

#pragma omp parallel shared(max_i_shared, max_j_shared, min_i_shared, min_j_shared) private(j, max_i, max_j, min_i, min_j)
	{
#pragma omp for reduction(+:total) nowait
		for (i = 0; i < size; i++)
			for (j = 0; j < size; j++)
			{
				total += matrix[i][j];
				if (matrix[i][j] > matrix[max_i][max_j])
				{
					max_i = i;
					max_j = j;
				}
				if (matrix[i][j] < matrix[min_i][min_j])
				{
					min_i = i;
					min_j = j;
				}
			}
#pragma omp critical
		{
// 			if (matrix[max_i][max_j] > matrix[max_i_shared][max_j_shared])
// 			{
// 				max_i_shared = max_i;
// 				max_j_shared = max_j;
// 			}
// 			if (matrix[min_i][min_j] > matrix[min_i_shared][min_j_shared])
// 			{
// 				min_i_shared = min_i;
// 				min_j_shared = min_j;
// 			}
		}
	}
	// implicit barrier

	end_time = omp_get_wtime();

	printf("the total is %d\n", total);
	printf("the maximum element is: %d on coordinates: (%d;%d)\n", matrix[max_i_shared][max_j_shared], max_i_shared, max_j_shared);
	printf("the minimum element is: %d on coordinates: (%d;%d)\n", matrix[min_i_shared][min_j_shared], min_i_shared, min_j_shared);
	printf("it took %g seconds\n", end_time - start_time);
}
