#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

static int
partition(int p, int r, double *data)
{
	double x = data[p];
	int k = p;
	int l = r + 1;
	double t;
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
seq_quick_sort(int p, int r, double *data)
{
	if (p < r)
	{
		int q = partition(p, r, data);
		seq_quick_sort(p, q - 1, data);
		seq_quick_sort(q + 1, r, data);
	}
}

static void
par_quick_sort(int p, int r, double *data, int low_limit)
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

int main(int argc, char *argv[])
{
	int i;
	int pthr;
	double *Data;
	int N, low_limit;
	double start_time, end_time;

	/*
     * Get input
     */
	if (argc < 4)
	{
		printf("Usage: qsort number_of_data number_of_threads low_limit\n");
		exit(1);
	}
	N = atoi(argv[1]);
	pthr = atoi(argv[2]);
	low_limit = atoi(argv[3]);
	printf("Number of Data: %d\n", N);
	printf("Number of Threads: %d\n", pthr);
	printf("Low Limit : %d\n", low_limit);
	printf("\n");

	/*
     * Generate the array
     */
	Data = (double *)malloc(sizeof(double) * N);
	if (Data == NULL)
	{
		printf("Error\n");
		exit(1);
	}
	for (i = 0; i < N; i++)
	{
		Data[i] = 1.1 * rand() * 5000 / RAND_MAX;
	}

	/*
     * Quick sort using OMP Task
     */
	omp_set_num_threads(pthr);
	omp_set_dynamic(0);
	start_time = omp_get_wtime();
#pragma omp parallel
	{
#pragma omp single nowait
		par_quick_sort(0, N - 1, &Data[0], low_limit);
	}
	end_time = omp_get_wtime();
	printf("Parallel quick_sort() Time: %g seconds\n", end_time - start_time);

	return 0;
}
