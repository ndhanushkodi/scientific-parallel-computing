#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define MAX 3 /* Max value to use in random population of matrices */ 


void randomizeMatrix(int row, int col, int* matrix);
void printMatrix(int row, int col, int* matrix);
void multiplyMatrices(int m, int n, int p, int* A, int* B, int* C);

/*
	Can change m, n, and p below in the main function,
	but they all must be the same value.

	Run with numprocs that is a divisor of n, m, and p PLUS ONE. So if n,m,p are 20, and 5 is a divisor of 20, run with 6 procs so one can be the master. 
*/
int main(int argc, char* argv[]){
	int *A, *B, *C;
	int *tempB;
	int m, n, p;

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int numprocs, pid, namelen;
	
	srand(time(NULL)); /* Call exactly once, to seed future rand() calls*/


	/*
		Setup MPI
	*/
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&pid);
	MPI_Get_processor_name(processor_name,&namelen);
	MPI_Status status;

	/*
		Initialize n, m, and p to same number
	*/
	n = 10000;
	m = 10000;
	p = 10000;

	//chunksize is the rows of A and C each process is computing from
	int chunksize = m/(numprocs-1);
	
	//will be used to hold the array index to start at for
	//each process
	int offset; 




	/*
		Initialize Variables
			-allocate space for A, B, and C according
			to whether you are a worker or master
	*/
	//Allocate space for square matrices A, B, and C 
	if(pid == 0){
		A = malloc(sizeof(int) * m * n);
		B = malloc(sizeof(int) * n * p);
		C = malloc(sizeof(int) * m * p);
	} else{
		//workers only need space for numRows/(numprocs-1) times the number of columns
		A = malloc(sizeof(int) * (m/(numprocs-1)) * n);
		B = malloc(sizeof(int) * n * p);
		C = malloc(sizeof(int) * (m/(numprocs-1)) * p);
	}

	if(!A || !B || !C){
		printf("Out of memory, decrease matrix dimensions, exiting...\n");
		return -1;
	}




	/*
		Communication
			-if master, initialize A and B, and send
			all of B to everyone, and send specific rows of
			A to everyone
			-if worker, receive 2 things from master, 
			the offset in A or C that I'm at, and the
			data from the chunk of rows of A.
	*/
	if(pid == 0){
		randomizeMatrix(m, n, A);
		randomizeMatrix(n, p, B);
		printMatrix(m, n, A);
		printMatrix(n, p, B);
		offset = 0;
		int i;
		for(i=1; i<numprocs; i++){
			MPI_Send(&offset, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
			MPI_Send(&A[offset], chunksize*n, MPI_INT, i, 1, MPI_COMM_WORLD);
			offset+=chunksize*n;
		}
	}
	
	//if pid is 0, this will broadcast B, otherwise,
	//every other process will receive B.
	MPI_Bcast(B, n*p, MPI_INT, 0, MPI_COMM_WORLD);

	//if worker, receive your part of A, then do 
	//matrix multiply for your chunk of A with B,
	//to compute your chunk of C 
	if(pid > 0){
		MPI_Recv(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
		MPI_Recv(&A[0], chunksize*n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		multiplyMatrices(m/(numprocs-1), n, p, A, B, C);
	}




	/*
		All processes must finish their part of 
		the computation before sending back to master
	*/
	MPI_Barrier(MPI_COMM_WORLD);




	/*
		Send your part of C to master
	*/
	if(pid > 0){
		MPI_Send(&offset, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
		MPI_Send(&C[0], chunksize*n, MPI_INT, 0, 4, MPI_COMM_WORLD);
	}
	/*Master receives chunks of C at each offset,
	and puts it all inside the "big" C which has 
	all the rows and columns of the product*/
	if(pid == 0){
		int i;
		for(i = 1; i<numprocs; i++){
			MPI_Recv(&offset, 1, MPI_INT, i, 3, MPI_COMM_WORLD, &status);

			MPI_Recv(&C[offset], chunksize*n, MPI_INT, i, 4, MPI_COMM_WORLD, &status);
		}
		//finally, print C
		printMatrix(m, p, C);
	}
	MPI_Finalize();





	/* Free memory allocated - there's no garbage collector! */
	free(A);
	free(B);
	free(C);
	
	return 0;
}

void randomizeMatrix(int row, int col, int* matrix){
	int i,j;
	for(i = 0; i < row; i++){
		for(j = 0; j < col; j++){
			matrix[i*col+j] = rand()%MAX;
			//matrix[i*col+j] = 1; //for testing, setting all values to 1.
		}
	}
}

void printMatrix(int row, int col, int* matrix){
	int i,j;
	printf("\n");
	for(i = 0; i < row; i++){
		for(j = 0; j < col; j++){
			printf("%d ", matrix[i*col+j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

/*
	Multiply matrices C=A*B. At each step k
*/
void multiplyMatrices(int m, int n, int p, int* A, int* B, int* C){
	int i, j, k;
	
	for(i = 0; i < m; i++){
		for(j = 0; j < p; j++){
			C[i*p+j] = 0;
			for(k = 0; k < n; k++) {

				C[i*p+j] += A[i*n+k] * B[k*p+j];
			}
		}
	}
}