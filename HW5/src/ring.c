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

		/* Fix nice print */
#ifdef NICEPRINT
		if (rank == ROOT)
		{
			printf("\n");
		}
		MPI_Barrier(MPI_COMM_WORLD);
		printf("I am proccess %d and my v is %d\n", rank, v);
		MPI_Barrier(MPI_COMM_WORLD);
#endif

		int max, min;
		int collection[2];
		/**
		 * collection[0]: max
		 * collection[1]: min
		 * */

		if (rank == ROOT)
		{
			collection[0] = v;
			collection[1] = v;
#ifdef NICEPRINT
			printf("\n[ ROOT ]: Ring started\n");
#endif
			MPI_Send(&collection[0], 2, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
			MPI_Recv(&collection[0], 2, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#ifdef NICEPRINT
			printf("\n[ ROOT ]: Ring has completed once. Max is %d and min is %d. Sending to next\n", collection[0], collection[1]);
#endif
			MPI_Send(&collection[0], 2, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		}
		else
		{
			MPI_Recv(&collection[0], 2, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			collection[0] = v < collection[0] ? collection[0] : v;
			collection[1] = v > collection[1] ? collection[1] : v;
#ifdef NICEPRINT
			printf("[P %d]: I have received message. My current max is: %d and min is %d. Sending to next\n", rank, collection[0], collection[1]);
#endif
			MPI_Send(&collection[0], 2, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
			MPI_Recv(&collection[0], 2, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#ifdef NICEPRINT
			printf("[P %d]: I have received message. Max is: %d and min is %d. Sending to next\n", rank, collection[0], collection[1]);
#endif
			if (rank < size - 1)
			{
				MPI_Send(&collection[0], 2, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
			}
			else
			{
#ifdef NICEPRINT
				printf("[P %d]: End of ring! Max is %d and min is %d\n\n", rank, collection[0], collection[1]);
#endif
			}
		}
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