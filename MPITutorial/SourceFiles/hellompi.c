/************************************************************************
* Write an MPI "hello World!" program using the appropriate MPI calls.
* This should resemble the output of the program seen in class last week
* during our intro to MPI lecture
************************************************************************/

/**************************
1.Include the necessary header files
**************************/
#include <stdio.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char message[20];
	MPI_Status status;
	int i,rank, size, tag=11;
  
	/**************************
	2.Initialize MPI 
	**************************/
 	MPI_Init(&argc, &argv);

	/**************************
	3.Find out my rank in the global communicator MPI_COMM_WORLD
	**************************/
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/**************************
	4.Find out the size of the global communicator MPI_COMM_WORLD
	**************************/
  	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/**************************
	5.Check the rank, and 
	**************************/

	if(rank == 0) {
		/**************************
		a.if the process is the master, then send a "Hello,World!" 
		message, in characters, to each of the workers using MPI_Send. 
		**************************/
		strcpy(message, "Hello,World!");
		for (i=1; i<size; i++) {
    		MPI_Send(message, 20, MPI_BYTE, i, 0, MPI_COMM_WORLD);
		}
	} 
	else {
		/**************************
		b.if the process is a worker, then receive the "Hello,World!" 
		message and print it out, using MPI_Recv.
		**************************/
    	MPI_Recv(message, 20, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status ); 
		printf( "Process %d : %.13s\n", rank, message);
	}

	/**************************
	6.Finalize MPI 
	**************************/
  	MPI_Finalize();
  
	return 0;
}