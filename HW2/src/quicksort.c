#include <omp.h>
#include <stdlib.h>

#define MAXLENGTH 1000000 /* Maximum length of list */
#define MAXWORKERS 24	 /* Maximum number of workers */
#define MINLOWERBOUND 1   /* Minimum lower bound */

/* Helper functions */
double drand(double low, double high)
{
	return ((double)rand() * (high - low)) / (double)RAND_MAX + low;
}

double copy(double mainList[], double copyList[])
{
	int i;
	for (i = 0; i < sizeof(copyList) / sizeof(copyList[0]); i++)
	{
		copyList[i] = mainList[i];
	}
}

/* Insertion sort is effective on smaller lists */
static void insertionSort(double arr[], int n)
{
	int i, key, j;
	for (i = 1; i < n; i++)
	{
		key = arr[i];
		j = i - 1;

		while (j >= 0 && arr[j] > key)
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}
}

/* Code for swaping two elements */
void swap(double *a, double *b)
{
	double temp = *a;
	*a = *b;
	*b = temp;
}

/* Partition code for quick sort */
static int partition(int low, int high, double arr[])
{
	int pivot = arr[high]; /* pivot */
	int i = (low - 1);	 // Index of smaller element

	int j;
	for (j = low; j <= high - 1; j++)
	{
		// If current element is smaller than or
		// equal to pivot
		if (arr[j] <= pivot)
		{
			i++; // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}
/* Parallel code for quick sort */
static void parallel_quicksort(int pivot, int high, double *list, int lower_bound)
{
	// 	if (pivot >= high)
	// 		return;

	// 	if (high - pivot < lower_bound)
	// 		return insertionSort(list, high - pivot);

	// 	int mid = partition(pivot, high, list);

	// #pragma omp task
	// 	parallel_quicksort(pivot, mid - 1, list, lower_bound);
	// #pragma omp task
	// 	parallel_quicksort(mid + 1, high, list, lower_bound);
}

/* Main: sets up all parameters */
int main(int argc, char *argv[])
{
	/* read command line args if any */
	int length = (argc > 1) ? atoi(argv[1]) : MAXLENGTH;
	int numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
	int lower_bound = (argc > 3) ? atoi(argv[3]) : MINLOWERBOUND;

	/* Set number of threads */
	omp_set_num_threads(numWorkers);

	/* Initialize list */
	double mainList[length];
	int i;
	for (i = 0; i < length; i++)
	{
		mainList[i] = drand(0.0, 100.0);
	}

	/* Keep original list untouched */
	double copyList[length];
	copy(mainList, copyList);

#ifdef VERBOSE
	printf("Unsorted list: [");
	for (i = 0; i < length - 1; i++)
	{
		printf("%f, ", copyList[i]);
	}
	printf("%f]\n", copyList[length - 1]);
#endif

	/* Start sorting list */
	double start_time, end_time;
	start_time = omp_get_wtime();
#pragma omp parallel
	{
#pragma single nowait
		{
			parallel_quicksort(0, length - 1, &copyList[0], lower_bound);
		}
	}
	end_time = omp_get_wtime();

#ifdef VERBOSE
	printf("Sorted list: [");
	for (i = 0; i < length - 1; i++)
	{
		printf("%f, ", copyList[i]);
	}
	printf("%f]\n", copyList[length - 1]);
#endif
	return 0;
}