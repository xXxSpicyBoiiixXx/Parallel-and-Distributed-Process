#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h> 


// Local matrix depening on MPI nodal region. 
void decomposeMPI(double **A, long n, int argc, char *argv[]) {

	

	MPI_Status status;  
	
	double time_start, time_end; 

	MPI_Init(&argc, &argv); 
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs); 

	time_start = MPI_Wtime(); 

}


