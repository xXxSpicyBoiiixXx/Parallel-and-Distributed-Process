#include <stdio.h> 
#include <stdlib.h> 
#include "sequential.h"

void decomposeSerial(double **A, long n) { 
		
	printf("Decomposition sequentially\n") ;
	
	for(long i = 0; i < n; i++) { 
		for(long j = i + 1; j < n; j++) { 
			A[i][j] = A[i][j]/A[i][i];
		}
		
		for(long k = i + 1; k < n; k++) { 
			for(long j = i + 1; j < n; j++) { 
				A[k][j] = A[k][j] - A[k][i] * A[i][j]; 
			}
		}
	}
}

int checkError1(double **A, long n) { 
	
	for(long i = 0; i < n; i++) { 
		for(long j = 0; j < n; j++) { 
			if(A[i][j] != 1) { 
				return 0; 
			} 
		}
	}
	return 1; 
}


void initializeLoop1(double **A, long n) {
	
	for(long i = 0; i < n; i++) { 
		for(long j = 0; j < n; j++) {
			if(i <= j) { 
				A[i][j] = i + 1; 
			} else { 
				A[i][j] = j + 1;
			}
		}
	}
}


int checkError2(double **A, long n) { 

	for(long i = 0; i < n; i++) { 
		if(A[i][i] != 1) { 
			return 0;
		}
		
		for(long j = 0; j < n; j++) { 
			if(i != j && A[i][j] != 2) { 
				return 0;
			}
		}
	}
	return 1;
}

void initializeLoop2(double **A, long n) { 
	
	long i, j, k;
	for(i = 0; i < n; i++) { 
		for( j = i; j < n; j++) { 
			if(i == j) { 
				k = i + 1; 
				A[i][j] = 4 * k - 3; 		
			} else { 
					A[i][j] = A[i][i] + 1; 
					A[j][i] = A[i][i] + 1;
			}
		}
	}	
}

double **getMatrix(long size, long loop) { 
	
	double **m = make2dmatrix(size); 
	
	switch(loop) { 
	case 1: 
		initializeLoop1(m, size);
		break; 
	case 2: 
		initializeLoop2(m, size); 
		break;
	default: 
		printf("Please enter either 1 or 2 for the loop varition..."); 
		exit(0); 
	}
	return m;
}

int checkSum(double **A, long size, long loop) { 
	
	switch(loop) { 
	case 1: 
		return checkError1(A, size); 
		break; 
	case 2: 
		return checkError2(A, size); 
	default: 
		printf("Invalid loop character in the check"); 
		exit(0); 
	}
}

double **make2dmatrix(long n) { 
	
	long i;
	double **m; 
	
	m = (double**)malloc(n*sizeof(double*)); 

	for(i = 0; i < n; i++) { 
		m[i] = (double*)malloc(n*sizeof(double)); 
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

void free2dmatrix(double ** A, long n) { 
	
	if(!A) { 
		return;
	}

	for(long i = 0; i < n; i++) { 
		free(A[i]); 
	}
	
	free(A); 
}


