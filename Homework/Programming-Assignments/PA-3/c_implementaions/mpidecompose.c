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

	int pid; 
	int partner; 
	int nprocs; 
	int i, j, k, p, min, max; 
	int nrows; 
	int ierr; 
	
	
	int data_tag = 9999; 
	int root_process = 0;

	double rowk[n];

	nrows = n/nprocs; 

	min = pid * nprocs;
	max = min + nrows - 1; 
	
	// Finsh logic here when you get the time, might not be that important 

	if(pid == nprocs - 1 && (n - (max + 1)) > 0) {
		max = n - 1;
	}

	for(k = 0; k < n; k++) { 
		if(k >= min && k <= max) { 
			
		}
	}
	
}


