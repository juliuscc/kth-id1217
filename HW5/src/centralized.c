#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>

#define MAXROUNDS 5

#define ROOT 0

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

int main(int argc, char *argv[])
{
	int numRounds = (argc > 1) ? atoi(argv[1]) : MAXROUNDS;

	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	srand(time(NULL) * rank);

	double start_time, end_time;
	if (rank == ROOT)
		start_time = read_timer();

	int i;
	for (i = 0; i < numRounds; i++)
	{
		/* Seed and get random number */
		int v = rand() % 100;

#ifdef NICEPRINT
		/* Fix nice print */
		if (rank == ROOT)
		{
			printf("\n");
		}
		MPI_Barrier(MPI_COMM_WORLD);
		printf("I am proccess %d and my v is %d\n", rank, v);
#endif

		/* Gather all v:s */
		int temp, max, min;
		int collection[2];
		int receive_buffer[size];
		MPI_Gather(&v, 1, MPI_INT, &receive_buffer[0], 1, MPI_INT, ROOT, MPI_COMM_WORLD);

		if (rank == ROOT)
		{
			max = v;
			min = v;
			int j;
			for (j = 1; j < size; j++)
			{
				max = max < receive_buffer[j] ? receive_buffer[j] : max;
				min = min > receive_buffer[j] ? receive_buffer[j] : min;
			}

#ifdef NICEPRINT
			printf("\n[ ROOT ]: Finished parsing v:s\n");
			printf("[ ROOT ]: The largest  number is: %d\n", max);
			printf("[ ROOT ]: The smallest number is: %d\n\n", min);
#endif

			collection[0] = max;
			collection[1] = min;

			/* MPI Broadcast is much faster and easier however it is cheating */
			for (j = 1; j < size; j++)
			{
				MPI_Send(&collection[0], 2, MPI_INT, j, 0, MPI_COMM_WORLD);
			}
		}
		else
		{
			MPI_Recv(&collection[0], 2, MPI_INT, ROOT, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		max = collection[0];
		min = collection[1];

		MPI_Barrier(MPI_COMM_WORLD);

#ifdef NICEPRINT
		printf("[ P 0%d ]: The largest  number is: %d\n", rank, max);
		MPI_Barrier(MPI_COMM_WORLD);
		if (rank == ROOT)
			printf("\n");
		MPI_Barrier(MPI_COMM_WORLD);
		printf("[ P 0%d ]: The smallest number is: %d\n", rank, min);
#endif
	}

	if (rank == ROOT)
		end_time = read_timer();

	MPI_Finalize();

	if (rank == ROOT)
	{
#ifdef NICEPRINT
		printf("%d rounds took %g seconds", numRounds, end_time - start_time);
#else
		printf("%g", end_time - start_time);
#endif
	}

	return 0;
}
