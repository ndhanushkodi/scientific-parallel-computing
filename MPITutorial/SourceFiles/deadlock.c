#include <stdio.h>
#include "mpi.h"

#define MSGLEN 2048

int main(int argc, char *argv[]) {
	int rank, i, dest, src, istag, iretag;
	float msg1[MSGLEN];
	float msg2[MSGLEN];
	MPI_Status recv_status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);  

	for (i = 0; i < MSGLEN; i++) {
		msg1[i] = 100;
		msg2[i] = -100;
	}
	
	if ( rank == 0 ) { 
		dest  = 1;
		src   = 1;
	} else if ( rank == 1 ) {
		dest  = 0;
		src   = 0;
	}

	printf("Task %d has sent the message\n", rank);
  
	MPI_Ssend(&msg1, MSGLEN, MPI_FLOAT, dest, 0, MPI_COMM_WORLD); 
	MPI_Recv(&msg2, MSGLEN, MPI_FLOAT, src, 0, MPI_COMM_WORLD, &recv_status);
  
	printf("Task %d has received the message\n", rank);
  
	MPI_Finalize();
	return 0;
}