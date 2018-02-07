#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

/* Helper function */
double drand(double low, double high)
{
	return ((double)rand() * (high - low)) / (double)RAND_MAX + low;
}

static int
partition(int p, int r, float *data)
{
	float x = data[p];
	int k = p;
	int l = r + 1;
	float t;
	while (1)
	{
		do
			k++;
		while ((data[k] <= x) && (k < r));
		do
			l--;
		while (data[l] > x);
		while (k < l)
		{
			t = data[k];
			data[k] = data[l];
			data[l] = t;
			do
				k++;
			while (data[k] <= x);
			do
				l--;
			while (data[l] > x);
		}
		t = data[p];
		data[p] = data[l];
		data[l] = t;
		return l;
	}
}

static void
seq_quick_sort(int p, int r, float *data)
{
	if (p < r)
	{
		int q = partition(p, r, data);
		seq_quick_sort(p, q - 1, data);
		seq_quick_sort(q + 1, r, data);
	}
}

static void
par_quick_sort(int p, int r, float *data, int low_limit)
{
	if (p < r)
	{
		if ((r - p) < low_limit) // Optimization. Sort small arrays sequentially
			seq_quick_sort(p, r, data);
		else
		{
			int q = partition(p, r, data);
#pragma omp task
			par_quick_sort(p, q - 1, data, low_limit);
#pragma omp task
			par_quick_sort(q + 1, r, data, low_limit);
		}
	}
}

/* Do one iteration and measure the time it took */
double oneIteration(float *originalArray, int length)
{
	double start_time, end_time;
	float *tempArray;
	int i;

	/* Create this iterations array */
	tempArray = (float *)malloc(sizeof(float) * length);
	if (tempArray == NULL)
	{
		printf("Error!\n");
		exit(1);
	}
	// for (i = 0; i < length; i++)
	// 	tempArray[i] = originalArray[i];

	// 	/* Do and measure one iteration */
	start_time = omp_get_wtime();
	// #pragma omp parallel
	// 	{
	// #pragma omp single nowait
	// 		par_quick_sort(0, length - 1, &tempArray[0], 1000);
	// 	}
	end_time = omp_get_wtime();

	// 	/* Free the array and return the time passed sorting it */
	// 	free(tempArray);
	return end_time - start_time;
}

int main(int argc, char *argv[])
{
	int workers[6] = {1, 2, 4, 8, 12, 24};
	int lengths[3] = {100000, 1000000, 10000000};
	int maxLength = lengths[sizeof(lengths) / sizeof(int)];

	/* Init main array */
	float *mainArray;
	mainArray = (float *)malloc(sizeof(float) * maxLength);
	if (mainArray == NULL)
	{
		printf("Error!\n");
		exit(1);
	}
	int i;
	for (i = 0; i < maxLength; i++)
		mainArray[i] = drand(0.0, 100.0);

	printf("Sorting list...\n");
	printf("Sorting list took %g seconds\n", oneIteration(&mainArray[0], lengths[0]));

	free(mainArray);
	// 	/*
	//      * Quick sort using OMP Task
	//      */
	// 	omp_set_num_threads(pthr);
	// 	omp_set_dynamic(0);
	// 	start_time = omp_get_wtime();
	// #pragma omp parallel
	// 	{
	// #pragma omp single nowait
	// 		par_quick_sort(0, N - 1, &Data[0], low_limit);
	// 	}
	// 	end_time = omp_get_wtime();
	// 	printf("Parallel quick_sort() Time: %g seconds\n", end_time - start_time);

	return 0;
}

// #include <omp.h>
// #include <stdlib.h>
// #include <stdio.h>

// #define MAXLENGTH 100000  /* Maximum length of list */
// #define MAXWORKERS 24	 /* Maximum number of workers */
// #define MINLOWERBOUND 100 /* Minimum lower bound */

// /* Helper functions */
// float drand(float low, float high)
// {
// 	return ((float)rand() * (high - low)) / (float)RAND_MAX + low;
// }

// static void copy(float mainList[], float copyList[])
// {
// 	int i;
// 	for (i = 0; i < sizeof(copyList) / sizeof(copyList[0]); i++)
// 	{
// 		copyList[i] = mainList[i];
// 	}
// }

// /* Insertion sort is effective on smaller lists */
// static void insertionSort(float arr[], int n)
// {
// 	int i, key, j;
// 	for (i = 1; i < n; i++)
// 	{
// 		key = arr[i];
// 		j = i - 1;

// 		while (j >= 0 && arr[j] > key)
// 		{
// 			arr[j + 1] = arr[j];
// 			j = j - 1;
// 		}
// 		arr[j + 1] = key;
// 	}
// }

// /* Partition code for quick sort */
// static int
// partition(int p, int r, float *data)
// {
// 	float x = data[p];
// 	int k = p;
// 	int l = r + 1;
// 	float t;
// 	while (1)
// 	{
// 		do
// 			k++;
// 		while ((data[k] <= x) && (k < r));
// 		do
// 			l--;
// 		while (data[l] > x);
// 		while (k < l)
// 		{
// 			t = data[k];
// 			data[k] = data[l];
// 			data[l] = t;
// 			do
// 				k++;
// 			while (data[k] <= x);
// 			do
// 				l--;
// 			while (data[l] > x);
// 		}
// 		t = data[p];
// 		data[p] = data[l];
// 		data[l] = t;
// 		return l;
// 	}
// }

// static void sequential_quicksort(int pivot, int high, float *list, int lower_bound)
// {
// 	if (pivot >= high)
// 		return;

// 	int mid = partition(pivot, high, list);
// 	sequential_quicksort(pivot, mid - 1, list, lower_bound);
// 	sequential_quicksort(mid + 1, high, list, lower_bound);
// }

// /* Parallel code for quick sort */
// static void parallel_quicksort(int pivot, int high, float *list, int lower_bound)
// {
// 	if (pivot >= high)
// 		return;

// 	if ((high - pivot) < lower_bound)
// 		return sequential_quicksort(pivot, high, list, lower_bound);

// 	int mid = partition(pivot, high, list);

// #pragma omp task
// 	parallel_quicksort(pivot, mid - 1, list, lower_bound);
// #pragma omp task
// 	parallel_quicksort(mid + 1, high, list, lower_bound);
// }

// /* Main: sets up all parameters */
// int main(int argc, char *argv[])
// {
// 	/* Read command line args if any */
// 	int length = (argc > 1) ? atoi(argv[1]) : MAXLENGTH;
// 	int numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
// 	int lower_bound = (argc > 3) ? atoi(argv[3]) : MINLOWERBOUND;

// 	/* Limit arguments to valid args */
// 	// if (length > MAXLENGTH)
// 	// 	length = MAXLENGTH;
// 	// if (numWorkers > MAXWORKERS)
// 	// 	numWorkers = MAXWORKERS;
// 	// if (lower_bound < MINLOWERBOUND)
// 	// 	lower_bound = MINLOWERBOUND;

// 	printf("List lenght: %d\n", length);
// 	printf("Number of workers: %d\n", numWorkers);
// 	printf("Lower bound where insertion sort begins: %d\n\n", lower_bound);

// 	/* Set number of threads */
// 	omp_set_num_threads(numWorkers);

// 	/* Initialize list */
// 	float *mainList;
// 	float *copyList;
// 	mainList = (float *)malloc(sizeof(float) * length);
// 	copyList = (float *)malloc(sizeof(float) * length);

// 	if (mainList == NULL || copyList == NULL)
// 	{
// 		printf("Error\n");
// 		exit(1);
// 	}

// 	int i;
// 	for (i = 0; i < length; i++)
// 	{
// 		// mainList[i] = drand(0.0, 100.0);
// 		mainList[i] = 1.1 * rand() * 5000 / RAND_MAX;
// 	}

// 	/* Keep original list untouched */
// 	for (i = 0; i < length; i++)
// 	{
// 		copyList[i] = mainList[i];
// 	}

// #ifdef VERBOSE
// 	printf("Unsorted list: [");
// 	for (i = 0; i < length - 1; i++)
// 	{
// 		printf("%f, ", copyList[i]);
// 	}
// 	printf("%f]\n", copyList[length - 1]);
// #endif

// 	/* Start sorting list */
// 	double start_time, end_time;
// 	start_time = omp_get_wtime();
// #pragma omp parallel
// 	{
// #pragma single nowait
// 		{
// 			parallel_quicksort(0, length - 1, &copyList[0], lower_bound);
// 		}
// 	}
// 	end_time = omp_get_wtime();

// #ifdef VERBOSE
// 	printf("Sorted list: [");
// 	for (i = 0; i < length - 1; i++)
// 	{
// 		printf("%f, ", copyList[i]);
// 	}
// 	printf("%f]\n", copyList[length - 1]);
// #endif

// 	printf("Sorted in %g seconds\n", end_time - start_time);
// 	return 0;
// }