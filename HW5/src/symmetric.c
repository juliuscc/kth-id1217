#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROOT 0 /* ROOT is only used for nice printing */

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
	if (rank == ROOT)
	{
		printf("\n");
	}
	MPI_Barrier(MPI_COMM_WORLD);

	/* TODO: Start measuring time from here */

	/* Gather all v:s and send my v to all */
	int receive_buffer[size];
	MPI_Allgather(&v, 1, MPI_INT, &receive_buffer[0], 1, MPI_INT, MPI_COMM_WORLD);

	/* Find max and min */
	int max = v;
	int min = v;
	int i;
	for (i = 0; i < size; i++)
	{
		max = max < receive_buffer[i] ? receive_buffer[i] : max;
		min = min > receive_buffer[i] ? receive_buffer[i] : min;
	}

	MPI_Barrier(MPI_COMM_WORLD);
	/* TODO: Stop measuring time here */

	printf("[ P 0%d ]: The largest  number is: %d\n", rank, max);
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == ROOT)
		printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	printf("[ P 0%d ]: The smallest number is: %d\n", rank, min);

	MPI_Finalize();
	return 0;
}