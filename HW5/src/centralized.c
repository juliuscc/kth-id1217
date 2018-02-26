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

	/* Gather all v:s */
	int temp, max, min;
	int collection[2];
	int receive_buffer[size];
	MPI_Gather(&v, 1, MPI_INT, &receive_buffer[0], 1, MPI_INT, ROOT, MPI_COMM_WORLD);

	if (rank == ROOT)
	{
		max = v;
		min = v;
		int i;
		for (i = 1; i < size; i++)
		{
			max = max < receive_buffer[i] ? receive_buffer[i] : max;
			min = min > receive_buffer[i] ? receive_buffer[i] : min;
		}

		printf("\n[ ROOT ]: Finished parsing v:s\n");
		printf("[ ROOT ]: The largest  number is: %d\n", max);
		printf("[ ROOT ]: The smallest number is: %d\n\n", min);

		collection[0] = max;
		collection[1] = min;

		/* MPI Gather is much faster and easier however it is cheating */
		for (i = 1; i < size; i++)
		{
			MPI_Send(&collection[0], 2, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Recv(&collection[0], 2, MPI_INT, ROOT, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	max = collection[0];
	min = collection[1];

	printf("[ P 0%d ]: The largest  number is: %d\n", rank, max);
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == ROOT)
		printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	printf("[ P 0%d ]: The smallest number is: %d\n", rank, min);

	MPI_Finalize();
	return 0;
}
