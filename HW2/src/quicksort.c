#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>

/* Helper function */
double drand(double low, double high)
{
	return ((double)rand() * (high - low)) / (double)RAND_MAX + low;
}

/* When list gets small */
static void insertionSort(float arr[], int n)
{
	int i, j;
	float temp;
	for (i = 1; i < n; i++)
	{
		temp = arr[i];
		j = i - 1;

		while (j >= 0 && arr[j] > temp)
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = temp;
	}
}

/* Meat of quick sort. Partitioning */
static int partition(int pivot_index, int high, float *data)
{
	float pivot = data[pivot_index];
	int k = pivot_index + 1;
	int l = high;
	float temp;

	while (k < l)
	{
		temp = data[k];
		data[k] = data[l];
		data[l] = temp;
		while (data[k] <= pivot)
			k++;
		while (data[l] > pivot)
			l--;
	}
	data[pivot_index] = data[l];
	data[l] = pivot;
	return l;
}

/* The parallel quicksort */
static void quicksort(int pivot, int high, float *list, int low_limit)
{
	if (pivot >= high)
		return;

	if ((high - pivot) < low_limit)
		return insertionSort(&list[pivot], high - pivot);

	int mid = partition(pivot, high, list);

#pragma omp task
	quicksort(pivot, mid - 1, list, low_limit);
#pragma omp task
	quicksort(mid + 1, high, list, low_limit);
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
	for (i = 0; i < length; i++)
		tempArray[i] = originalArray[i];

	/* Do and measure one iteration */
	start_time = omp_get_wtime();
#pragma omp parallel
	{
#pragma omp single nowait
		quicksort(0, length - 1, &tempArray[0], 1000);
	}
	end_time = omp_get_wtime();

#ifdef RUNONCE
	printf("Unsorted list: [");
	for (i = 0; i < length - 1; i++)
	{
		printf("%f, ", originalArray[i]);
	}
	printf("%f]\n", originalArray[length - 1]);

	printf("Sorted list: [");
	for (i = 0; i < length - 1; i++)
	{
		printf("%f, ", tempArray[i]);
	}
	printf("%f]\n", tempArray[length - 1]);

#endif

	/* Free the array and return the time passed sorting it */
	free(tempArray);
	return end_time - start_time;
}

/* Swap function for bubble sort */
void swap(double *xp, double *yp)
{
	double temp = *xp;
	*xp = *yp;
	*yp = temp;
}

/* An optimized version of bubble sort */
void sort(double arr[], int n)
{
	int i, j;
	bool swapped;

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

double medianTime(float *originalArray, int numWorkers, int length)
{
	int i;
	int iterations = 10;
	double times[iterations];

	/* Set threads */
	omp_set_num_threads(numWorkers);

	/* Run interations */
	for (i = 0; i < iterations; i++)
		times[i] = oneIteration(&originalArray[0], length);

	/* Calc median */
	sort(times, iterations);
	return times[iterations / 2];
}

int main(int argc, char *argv[])
{
	omp_set_dynamic(0);
	int workers[6] = {1, 2, 4, 8, 12, 24};
	int lengths[3] = {100000, 1000000, 10000000};
	int maxLength = lengths[sizeof(lengths) / sizeof(int) - 1];

	/* Init main array */
	float *mainArray;
	mainArray = (float *)malloc(sizeof(float) * maxLength);
	if (mainArray == NULL)
	{
		printf("Error!\n");
		exit(1);
	}
	int i, j;
	for (i = 0; i < maxLength; i++)
		mainArray[i] = drand(0.0, 100.0);

#ifdef RUNONCE
	oneIteration(&mainArray[0], 10);
#else
	/* Print start stuff */
	printf("\nWorkers = [1, 2, 4, 8, 12, 24]\n");
	printf("Lengths of list = [100000, 1000000, 10000000]\n");
	printf("Table:\n");
	printf("|Workers\\Length |     100000    |    1000000    |    10000000   |\n");
	printf("|---------------|---------------|---------------|---------------|\n");

	/* Run benchmark */
	for (i = 0; i < sizeof(workers) / sizeof(workers[0]); i++)
	{
		printf("|%d\t\t", workers[i]);
		for (j = 0; j < sizeof(lengths) / sizeof(lengths[0]); j++)
		{
			printf("|%g\t", medianTime(&mainArray[0], workers[i], lengths[j]));
		}
		printf("|\n");
	}
#endif
	/* Free main array and return */
	free(mainArray);
	return 0;
}