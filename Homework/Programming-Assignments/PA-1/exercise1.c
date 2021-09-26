/*
 * FILE:
 * USAGE:
 * DESCRIPTION: 
 * OPPTIONS:
 * REQUIREMENTS: 
 * BUGS: 
 * AUTHOR: xXxSpicyBoiiixXx
 * ORGANIZATION: Illinois Institute of Technology
 * VERSION: 1.0 
 * CREATED: 09/20/2021
 * REVISION: 
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define NUM_ELEMENTS 10

static void print_array(int * arr, size_t len) { 
	for(int i = 0; i < len; i++)
		printf("%d", arr[i]); 
	printf("\n");	
}

// comparison? 
static int compare_int(const void *a, const void *b)
{
	return (*(int *) a - *(int *) b); 
}

// unfinised looking thorugh code at the momnet
// is merge sort the way to go?
// how to make this represetnated for multiple threads????  
static void merge(int *a, int numel_a, int *b, int numel_b)
{
	int *sorted = (int *) malloc((numel_a + numel_b) * sizeof *a);
	int i, a_i = 0, b_i = 0; 
	
	for (i = 0; i < (numel_a + numel_b); i++) {
		if(a_i < numel_a && b_i < numel_b) {
			if(a[a_i] < b[b_i]) { 
			sorted[i] = a[a_i];
			a_i++;		
		}
		else {
			sorted[i] = b[b_i];
			b_i++;
		}}
	else { 
		if(a_i < numel_a) {
		sorted[i] = a[a_i];
		a_i++;}	
 else {
	sorted[i] = b[b_i];
	b_i++;
}
}
}

memcpy(a, sorted, (numel_a + numel_b) * sizeof *sorted);
free(sorted);
}

int main(int argc, char **argv) 
{
	int rank, size, data[NUM_ELEMENTS]; 
	MPI_Init(&argc, &argv); 
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	MPI_Comm_size(MPI_COMM_WORLD, &size); 

	if(size != 2) { 
		printf("Please run with exactly 2 rankss\n"); 
		MPI_Finalize(); 
		exit(0); 
	} 

	int first_half = floor((double) NUM_ELEMENTS / 2); 
	int second_half = NUM_ELEMENTS - first_half; 

	srand(0); 

	if(rank == 0) {
		int i; 
		printf("Unsorted:\t");
		
		print_array(data, NUM_ELEMENTS); 
	
	// sends data to the other rank
	MPI_Send(&data[first_half], second_half, MPI_INT, 1, 0, MPI_COMM_WORLD); 
	
	// sorts the first half
	qsort(data, first_half, sizeof(int), compare_int); 

	// receive sorted second half of the data
	MPI_Recv(&data[first_half], second_half, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

	// merging the two sorted arrays
	merge(data, first_half, &data[first_half], second_half);

	printf("Sorted:\t\t"); 
	
	for(i = 0; i < NUM_ELEMENTS; i++)
		printf("%d ", data[i]);
	printf("\n"); 

	} else if (rank == 1) {
		
	// receives half of the data 
	MPI_Recv(data, second_half, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	// sort the received data 
	qsort(data, second_half, sizeof(int), compare_int); 
	
	// send back sorted data 
	MPI_Send(data, second_half, MPI_INT, 0, 0, MPI_COMM_WORLD);	
 	}

MPI_Finalize();

return 0;

}
