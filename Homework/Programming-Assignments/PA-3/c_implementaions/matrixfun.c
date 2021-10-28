#include <stdlib.h> 
#include <stdio.h>

double **make2dmatrix(long n) { 
	
	double **m;
	m = (double **)malloc(n * sizeof(double*)); 

	for(long i = 0; i < n; i++) { 
		m[i] = (double*)malloc(n * sizeof(double));
	}

	return m;
}

void printMatrix(double **A, long n) { 
	
	for(long i = 0; i < n; i++) { 
		for(long j = 0; j < n; j++) { 
			printf("%f ", A[i][j]); 
		}
		
	printf("\n"); 
	
	}
}

void free2dmatrix(double **A, long n) { 
	
	if(!A) {
		return;
	}

	for(long i = 0; i < n; i++) { 
		free(A[i]);
	}

	free(A);
}	


