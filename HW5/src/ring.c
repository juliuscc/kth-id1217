#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROOT 0

int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* Seed and get random number */
	srand(time(NULL) * rank);
	int v = rand() % 100;

	/* Fix nice print */
	if (rank == ROOT)
	{
		printf("\n");
	}
	MPI_Barrier(MPI_COMM_WORLD);
	printf("I am proccess %d and my v is %d\n", rank, v);
	MPI_Barrier(MPI_COMM_WORLD);

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
		printf("\n[ ROOT ]: Ring started\n");
		MPI_Send(&collection[0], 2, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		MPI_Recv(&collection[0], 2, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("\n[ ROOT ]: Ring has completed once. Max is %d and min is %d. Sending to next\n", collection[0], collection[1]);
		MPI_Send(&collection[0], 2, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Recv(&collection[0], 2, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		collection[0] = v < collection[0] ? collection[0] : v;
		collection[1] = v > collection[1] ? collection[1] : v;
		printf("[P %d]: I have received message. My current max is: %d and min is %d. Sending to next\n", rank, collection[0], collection[1]);
		MPI_Send(&collection[0], 2, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
		MPI_Recv(&collection[0], 2, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("[P %d]: I have received message. Max is: %d and min is %d. Sending to next\n", rank, collection[0], collection[1]);
		if (rank < size - 1)
			MPI_Send(&collection[0], 2, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
		else
			printf("[P %d]: End of ring! Max is %d and min is %d\n", rank, collection[0], collection[1]);
	}

	MPI_Finalize();
	return 0;
}