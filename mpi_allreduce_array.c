/** 2016/11/12 (c) BassyKuo
 * Usage:
 *		mpicc -o mpi_allreduce_array mpi_allreduce_array.c
 *		mpirun -n <num_process> ./mpi_allreduce_array
 * For example:
 *		$ mpicc -o mpi_allreduce_array mpi_allreduce_array.c 
 * 		$ mpirun -n 2 mpi_allreduce_array
 * 		(0) before: 0
 * 		(0) before: 1
 * 		(0) before: 2
 * 		(0) before: 3
 * 		(0) before: 4
 * 		(0) before: 5
 * 		(0) before: 6
 * 		(0) before: 7
 * 		(0) before: 8
 * 		(0) before: 9
 * 		(1) before: 0
 * 		(1) before: 1
 * 		(1) before: 2
 * 		(1) before: 3
 * 		(1) before: 4
 * 		(1) before: 5
 * 		(1) before: 6
 * 		(1) before: 7
 * 		(1) before: 8
 * 		(1) before: 9
 * 		0
 * 		2
 * 		4
 * 		6
 * 		8
 * 		10
 * 		12
 * 		14
 * 		16
 * 		18
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

#define COUNT 10

int main(int argc, char** argv)
{
	int rank, numtasks;
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);

	int j = 0;
	int *i = (int*) malloc (sizeof(int) * COUNT);
	memset (i, 0, sizeof(int) * COUNT);
	for (j=0; j<COUNT; ++j) 
		printf ("(%d) before: %d\n",rank, i[j] = j);


	int *recv = (int*) malloc (sizeof(int) * COUNT);
	MPI_Allreduce (i, recv, COUNT, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	if (rank == 0)
		for (j=0; j<COUNT; ++j)
			printf ("%d\n", recv[j]);

	MPI_Finalize();
	return 0;
}
