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

double oneIteration(int arr[])
{
	int i, j, total = 0;

	int max_i = 0;
	int max_j = 0;
	int min_i = 0;
	int min_j = 0;
	int max_i_shared = 0;
	int max_j_shared = 0;
	int min_i_shared = 0;
	int min_j_shared = 0;

	start_time = omp_get_wtime();

#pragma omp parallel shared(max_i_shared, max_j_shared, min_i_shared, min_j_shared) private(j) firstprivate(max_i, max_j, min_i, min_j)
	{
#pragma omp for reduction(+:total) nowait
		for (i = 0; i < size; i++)
			for (j = 0; j < size; j++)
			{
				total += arr[i][j];
				if (arr[i][j] > arr[max_i][max_j])
				{
					max_i = i;
					max_j = j;
				}
				if (arr[i][j] < arr[min_i][min_j])
				{
					min_i = i;
					min_j = j;
				}
			}
#pragma omp critical
		{
			if (arr[max_i][max_j] > arr[max_i_shared][max_j_shared])
			{
				max_i_shared = max_i;
				max_j_shared = max_j;
			}
			if (arr[min_i][min_j] > arr[min_i_shared][min_j_shared])
			{
				min_i_shared = min_i;
				min_j_shared = min_j;
			}
		}
	}

	end_time = omp_get_wtime();

	return end_time - start_time;
}

void swap(double *xp, double *yp)
{
	double temp = *xp;
	*xp = *yp;
	*yp = temp;
}

// An optimized version of Bubble Sort
void sort(double arr[])
{
	int i, j;
	bool swapped;

	int n = sizeof(arr)/sizeof(arr[0])

	for (i = 0; i < n - 1; i++)
	{
		swapped = false;
		for (j = 0; j < n - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				swap(&arr[j], &arr[j + 1]);
				swapped = true;
			}
		}

		// IF no two elements were swapped by inner loop, then break
		if (swapped == false)
			break;
	}
}

double medianTime(int numWorkers, int size)
{
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

	int iterations = 10;
	double times[iterations];
	int i;
	for(i = 0; i < iterations; i++){
		times[i] = oneIteration(matrix);
	}
	// sort times
	sort(times)
	return times[iterations/2];

}

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{

	/* read command line args if any */
	size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
	numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
	if (size > MAXSIZE)
		size = MAXSIZE;
	if (numWorkers > MAXWORKERS)
		numWorkers = MAXWORKERS;

	double median = medianTime(numWorkers, size)

	printf("Median time: %g seconds\n", median);
}