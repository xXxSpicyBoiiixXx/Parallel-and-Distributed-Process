#include <stdio.h>
#include <stdlib.h> 
#include <omp.h>
#include <time.h>
//#include "openmp.h"

//void decomposeOpenMP(double **A, long n); 
//void initializeLoop1(double **A, long n);
//void initializeLoop2(double **A, long n); 
void printMatrix(double **A, long n); 
void free2dmatrix(double **A, long n); 

//int checkError1(double **A, long n);
//int checkError2(double **A, long n); 
//int checkSum(double **A, long size, long loop); 

double **make2dmatrix(long n);
//double **getMatrix(long size, long loop); 

long matrix_size; 
long variation;

int main(int argc, char *argv[]) { 
	
	clock_t start, end;
	double time_spent; 
	double **matrix;

	if(argc != 4) { 
		printf("Enter the size of the matrix (N x N) where N = "); 
		scanf("%lu", &matrix_size); 

		printf("Enter the variation number = "); 
		scanf("%lu", &variation); 
	} else { 
		matrix_size = atol(argv[1]); 
		variation = atol(argv[2]); 
	}

	//long num_threads = atol(argv[3]); 
	/*
	if(num_threads < 1) { 
		num_threads = 5;
	}*/

	omp_set_num_threads(2); 

	matrix = getMatrix(matrix_size, variation); 

	printMatrix(matrix, matrix_size);

	start = clock(); 
	
	decomposeOpenMP(matrix, matrix_size); 

	end = clock(); 

	time_spent = ((double)(end - start)) / CLOCKS_PER_SEC; 

	printMatrix(matrix, matrix_size); 
	printf("\n"); 
	printf("Size of Matrix :%lu \n", matrix_size);
	printf("Loop Variation Number : %lu\n", variation);
	printf("%s", checkSum(matrix, matrix_size, variation) == 1? "Decomposition successful... \n":"Decomposition failed...\n"); 
	printf("computation Time: %f seconds\n", time_spent); 

	free2dmatrix(matrix, matrix_size);

	return 0;	

}

void decomposeOpenMP(double **A, long n) {
    
        int nprocs;
        int pid = 0;
        
        long i, j, k;
        long rows, min, max;

        printf("Decomposition OpenMP\n");

#pragma omp parallel shared(A, n, nprocs) private(i, j, k, rows, pid, min, max)
        {
#ifdef _OPENMP
        nprocs = omp_get_num_threads();
#endif

#ifdef _OPENMP
        pid = omp_get_thread_num();
#endif

    printf("1. I am proc no %d out of %d\n", pid, nprocs);

    rows = n/nprocs;
    min = pid * rows;
    max = min + rows - 1;
    
    if(pid == nprocs - 1 && (n - (max + 1)) > 0) {
        max = n - 1;
    }

    for(i = 0; i < n; i++) {
        if(i >= min && i <= max) {
            for(j = i + 1; j < n; j++) {
                A[i][j] = A[i][j]/A[i][i];
            }
        }

#pragma omp barrier

    for(k = (((i + 1) > min) ? (i + 1) : min); k <= max; k++) {
            for(j = i + 1; j < n; j++) {
                A[k][j] = A[k][j] - A[k][i] * A[i][j];
            }
        }
    }
}
}

int checkError1(double **A, long n) {
    long i,j;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
               if(A[i][j] != 1) {
                return 0;
            }
        }
    }

return 1;
}

void initializeLoop1(double **A, long n) {

    long i, j;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(i <= j) {
                A[i][j] = i + 1;
            } else {
                A[i][j] = j + 1;
            }
        }
    }
}

int checkError2(double **A, long n) {
    long i, j;
    for(i = 0; i < n; i++) {
        if(A[i][i] != 1) {
            return 0;
        }

        for(j = 0; j < n; j++) {
            if(i != j && A[i][j] != 2) {
                return 0;
            }
        }
    }
    return -1;
}

void initializeLoop2(double **A, long n) {

    long i, j, k;

    for(i = 0; i < n; i++) {
        for(j = i; j < n; j++) {
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
        printf("Please enter etiher 1 or 2 for the loop variation number.");
        exit(0);
    }
    return 0;
}


int checkSum(double **A, long size, long loop) {
    switch(loop) {
    case 1:
        return checkError1(A, size);
        break;
    case 2:
        return checkError2(A, size);
        break;
    default:
        printf("Invalid loop character in checksum");
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

