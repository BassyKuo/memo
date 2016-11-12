/** 2016/11/12 (c) BassyKuo
 * Usage:
 *		mpicc -o mpi_create_struct_type mpi_create_struct_type.c
 *		mpirun mpi_create_struct_type
 * For example:
 *		$ mpicc mpi_create_struct_type.c 
 *		$ mpirun -n 2 ./a.out 
 *	 	(1) i = 120, j = 240, repeat = 330
 *	 	before:(0) i = 0, j = 0, repeat = 0
 *	 	after: (0) i = 120, j = 240, repeat = 330
 */

#include <stdio.h>
#include <stddef.h>
#include "mpi.h"
typedef struct MsgData
{
	int i;
	int j;
	int repeat;
} Msg;

int main (int argc, char** argv) {
	int numtasks, rank;

	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);

	/* create a type for struct MsgData */
	const int	nitems = 3;
	int			blocklengths[3] = {1,1,1};
	MPI_Datatype	types[3] = {MPI_INT, MPI_INT, MPI_INT};
	MPI_Datatype	mpi_msg_type;
	MPI_Aint		offsets[3];

	offsets[0] = offsetof(Msg, i);
	offsets[1] = offsetof(Msg, j);
	offsets[2] = offsetof(Msg, repeat);

	MPI_Type_create_struct (nitems, blocklengths, offsets, types, &mpi_msg_type);
	MPI_Type_commit		   (&mpi_msg_type);

	Msg mail;
	mail.i = 0;
	mail.j = 0;
	mail.repeat = 0;
	if (rank == 0) {
//		printf ("before:(%d) i = %d, j = %d, repeat = %d\n", rank, mail.i, mail.j, mail.repeat);
		MPI_Recv (&mail, 1, mpi_msg_type, 1, 11, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//		printf ("after: (%d) i = %d, j = %d, repeat = %d\n", rank, mail.i, mail.j, mail.repeat);
	}
	else {
		mail.i = 120;
		mail.j = 240;
		mail.repeat = 330;
//		printf ("(%d) i = %d, j = %d, repeat = %d\n", rank, mail.i, mail.j, mail.repeat);
		MPI_Send (&mail, 1, mpi_msg_type, 0, 11, MPI_COMM_WORLD);
	}
	MPI_Finalize();

	return 0;
}
