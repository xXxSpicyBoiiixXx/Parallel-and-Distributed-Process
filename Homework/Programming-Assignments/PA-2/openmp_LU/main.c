#include <stdio.h>
#include <stdlib.h> 
#include <omp.h>
#include <time.h>
//#include "openmp.h"

void decomposeOpenMP(double **A, long n); 
void initializeLoop1(double **A, long n);
void initializeLoop2(double **A, long n); 
void printMatrix(double **A, long n); 
void free2dmatrix(double **A, long n); 

int checkError1(double **A, long n);
int checkError2(double **A, long n); 
int checkSum(double **A, long size, long loop); 

double **make2dmatrix(long n);
double **getMatrix(long size, long loop); 

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

