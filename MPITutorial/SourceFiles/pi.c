/*  example from MPICH  */
#include "mpi.h"
#include <stdio.h>
#include <math.h>

#define true 1
#define false 0
#define PI25DT  3.141592653589793238462643
#define NUM_RECTANGLES 10000

double f(double);

double f(double x)
{
	return (4.0 / (1.0 + x*x));
}

int main(int argc,char *argv[]) {
	int done = false, n = 0, myid, numprocs, i;
	double mypi, pi, h, sum, x;
	double startwtime = 0.0, endwtime;
	int  namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Get_processor_name(processor_name,&namelen);

	printf("Process %d of %d on %s\n", myid, numprocs, processor_name);

	if (myid == 0) {
		n = NUM_RECTANGLES;
		startwtime = MPI_Wtime();
	}
	
	/* Send n to all processes in global communicator */
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	/* h is 1/n, looking at interval (0,1) */
	h   = 1.0 / (double) n;
	sum = 0.0;
	
	/* Walk through every numproc-th rectangle (the ones assigned to this processor)
		 and find its area */
	for (i = myid + 1; i <= n; i += numprocs) {
		/* x is the mid point of the current rectangle */
		x = h * ((double) i - 0.5);
		/* Find the value of the function at the current x */
		sum += f(x);
	}
	/* multiply by the width of each rectangle to get the area */
	mypi = h * sum;

	/* Use the MPI API to determine what this function is doing */
	MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (myid == 0) {
		printf("pi is approximately %.16f, Error is %.16f\n",
		pi, fabs(pi - PI25DT));
		endwtime = MPI_Wtime();
		printf("wall clock time = %f\n", endwtime-startwtime);	       
		fflush( stdout );
	}


	MPI_Finalize();
	return 0;
}