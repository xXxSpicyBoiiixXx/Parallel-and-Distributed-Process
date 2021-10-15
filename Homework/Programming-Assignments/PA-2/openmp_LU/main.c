/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

double **make2dmatrix(long n);
void free2dmatrix(double ** M, long n);
void printMatrix(double **A, long n);

long matrix_size;
long variation;

void decomposeOpenMP(double **A, long n)
{
	printf("Decompose OpenMP\n");

	long i, j, k, rows,min,max;
	int pid=0;
	int nprocs;

#pragma omp parallel shared(A,n,nprocs) private(i,j,k,pid,rows,min,max)
{
double **getMatrix(long size, long loop); 

long matrix_size; 
long variation;

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
		rows = n/nprocs;
		min = pid * rows;
		max = min + rows - 1;

		if(pid==nprocs-1 && (n-(max+1))>0)
			max=n-1;

		for(k=0;k<n;k++){
			if(k>=min && k<=max){
				for(j=k+1;j<n;j++){
					A[k][j] = A[k][j]/A[k][k];
				}
			}

#pragma omp barrier
			for(i=(((k+1) > min) ? (k+1) : min);i<=max;i++){
				for(j=k+1;j<n;j++){
					A[i][j] = A[i][j] - A[i][k] * A[k][j];
				}
			}
		}
	}
}

int checkError1(double **A, long n)
{
	long i, j;
	for (i=0;i<n;i++)
	{
		for (j=0;j<n;j++)
			if(A[i][j]!=1){
				return 0;
			}
	}
	return 1;
}

void initializeLoop1(double **A, long n)
{
	long i, j;
	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			if(i<=j )
				A[i][j]=i+1;
			else
				A[i][j]=j+1;

		}
	}
}


int checkError2(double **A, long n)
{
	long i,j;
	for(i=0;i<n;i++){
		if(A[i][i]!=1){
			return 0;
		}
		for(j=0;j<n;j++){
			if(i!=j && A[i][j]!=2){
				return 0;
			}
		}
	}
	return 1;
}

void initializeLoop2(double **A,long n){
	long i,j, k;
	for(i=0;i<n;i++){
		for(j=i;j<n;j++){
			if(i==j){
				k=i+1;
				A[i][j]=4*k-3;
			}
			else{
				A[i][j]=A[i][i]+1;
				A[j][i]=A[i][i]+1;
			}
		}
	}
}


double **getMatrix(long size,long loop)
{
	double **m=make2dmatrix(size);
    
	switch(loop){
	case 1:
		initializeLoop1(m,size);
		break;
	case 2:
		initializeLoop2(m,size);
		break;
	default:
		printf("Please eneter either 1 or 2 for the loop variation...");
		exit(0);
	}
	return m;
}

int checkSum(double **A, long size, long loop){
	switch(loop){
	case 1:
		return checkError1(A,size);
		break;
	case 2:
		return checkError2(A,size);
		break;
	default:
		printf("Invalid loop character in the cehck");
		exit(0);
	}
}

int main(int argc, char *argv[]){
    
    clock_t start, end;
    double time_spent;
    long num_threads;
    
	if(argc !=4){
		printf("Enter the size of matrix (N x N) where N = ");
		scanf("%lu",&matrix_size);

		printf("Enter the variation number = ");
		scanf("%lu",&variation);
        
        printf("Enter the number of threads");
        scanf("%lu",&num_threads);
	}
	else{
		matrix_size=atol(argv[1]);
		variation=atol(argv[2]);
        num_threads = atol(argv[3]);
	}

	if(num_threads<1){
		num_threads=5;
	}
    
	omp_set_num_threads(num_threads);

	double **matrix=getMatrix(matrix_size, variation);

	printMatrix(matrix,matrix_size);

	start = clock();

	decomposeOpenMP(matrix,matrix_size);

	end = clock();
    
	time_spent = ((double)(end - start)) / CLOCKS_PER_SEC;

	printMatrix(matrix,matrix_size);
    printf("\n");
	printf("Size of Matrix :%lu \n",matrix_size);
	printf("Loop Variation Number : %lu\n",variation);
	printf("Number of Procs : %lu\n",num_threads);
	printf("%s",checkSum(matrix,matrix_size,variation)==1? "Decomposition successful... \n":"Decomposition failed... \n");
	printf("Computation Time : %f seconds\n",time_spent);

	free2dmatrix(matrix,matrix_size);
	return 0;
}


double **make2dmatrix(long n)
{
	long i;
	double **m;
	m = (double**)malloc(n*sizeof(double*));
	for (i=0;i<n;i++)
		m[i] = (double*)malloc(n*sizeof(double));
	return m;
}

void printMatrix(double **A, long n)
{
	long i, j;
	for (i=0;i<n;i++)
	{
		for (j=0;j<n;j++)
			printf("%f ",A[i][j]);
		printf("\n");
	}
}

void free2dmatrix(double ** A, long n)
{
	long i;
	if (!A) return;
	for(i=0;i<n;i++)
		free(A[i]);
	free(A);
}
