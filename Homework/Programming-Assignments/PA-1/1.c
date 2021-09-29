/*
 * FILE:
 * USAGE:
 * DESCRIPTION: 
 * OPTIONS:
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

int main(int argc, char** argv) {
    int numProcs, id, globalArraySize, localArraySize, height;
    int *localArray, *globalArray;
    double startTime, localTime, totalTime;
    double zeroStartTime, zeroTotalTime, processStartTime, processTotalTime;;
    int length = -1;
    char myHostName[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    MPI_Get_processor_name (myHostName, &length); 

    // check for odd processes
    powerOfTwo(id, numProcs);

    // get size of global array
    getInput(argc, argv, id, numProcs, &globalArraySize);

    // calculate total height of tree
    height = log2(numProcs);

    // if process 0, allocate memory for global array and fill with values
    if (id==0){
		globalArray = (int*) malloc (globalArraySize * sizeof(int));
		fillArray(globalArray, globalArraySize, id);
		//printList(id, "UNSORTED ARRAY", globalArray, globalArraySize);  // Line A
	}
	
    // allocate memory for local array, scatter to fill with values and print
    localArraySize = globalArraySize / numProcs;
    localArray = (int*) malloc (localArraySize * sizeof(int));
    MPI_Scatter(globalArray, localArraySize, MPI_INT, localArray, 
		localArraySize, MPI_INT, 0, MPI_COMM_WORLD);
    //printList(id, "localArray", localArray, localArraySize);   // Line B 
    
    //Start timing
    startTime = MPI_Wtime();
    //Merge sort
    if (id == 0) {
		zeroStartTime = MPI_Wtime();
		globalArray = mergeSort(height, id, localArray, localArraySize, MPI_COMM_WORLD, globalArray);
		zeroTotalTime = MPI_Wtime() - zeroStartTime;
		printf("Process #%d of %d on %s took %f seconds \n", 
			id, numProcs, myHostName, zeroTotalTime);
	}
	else {
		processStartTime = MPI_Wtime();
	        mergeSort(height, id, localArray, localArraySize, MPI_COMM_WORLD, NULL);
		processTotalTime = MPI_Wtime() - processStartTime;
		printf("Process #%d of %d on %s took %f seconds \n", 
			id, numProcs, myHostName, processTotalTime);
	}
    //End timing
    localTime = MPI_Wtime() - startTime;
    MPI_Reduce(&localTime, &totalTime, 1, MPI_DOUBLE,
        MPI_MAX, 0, MPI_COMM_WORLD);

    if (id == 0) {
		//printList(0, "FINAL SORTED ARRAY", globalArray, globalArraySize);  // Line C
		printf("Sorting %d integers took %f seconds \n", globalArraySize,totalTime);
		free(globalArray);
	}

    free(localArray);  
    MPI_Finalize();
    return 0;
}

/* 
#define NUM_ELEMENTS 10


// Will be keeping both either ints or unsigned ints. 
static void print_array(int * arr, int len) { 
	for(int i = 0; i < len; i++)
		printf("%d ", arr[i]);
	printf("\n");	
}

static inline void init_array(int * a, int len) {
	for(int i = 0; i < len; i++) 
		a[i] = rand() % NUM_ELEMENTS;
}

static inline void usage(char * prog) { 
	printf("Usage: %s -n [number of processes]", prog); 
}

// comparison <--- fix for n ranks.  
static int compare_int(const void *a, const void *b)
{
	return (*(int *) a - *(int *) b); 
}

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
	int data[NUM_ELEMENTS]; 
	
	int my_id, root_process, ierr, num_procs; 
		
	ierr =  MPI_Init(&argc, &argv);

	root_process = 0; 
	
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id); 
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs); 
	
	if(my_id == root_process) { 
		printf("Unsorted:\t"); 
		init_array(data, NUM_ELEMENTS); 
		print_array(data, NUM_ELEMENTS); 
	}

}

/*
int main(int argc, char **argv) 
{
	int rank, size, data[NUM_ELEMENTS]; 

	int avg_data_per_process, num_procs; 

	int data_tag = 1001;

	int starting_index, ending_index, data_to_be_sent;
	
	MPI_Init(&argc, &argv); 
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	MPI_Comm_size(MPI_COMM_WORLD, &size); 

// Not sure what to do wit these yet..... we shall see!
//	size_t len; 	
//	len = atoll(argv[1]);

	if(argc < 1) { 
		usage(argv[0]);
		MPI_Finalize(); 
		exit(EXIT_SUCCESS); 	
	}
		
	if(size != 2) { 
		printf("Please run with exactly 2 rankss\n"); 
		MPI_Finalize(); 
		exit(0); 
<<<<<<< HEAD
	} 
=======
	}
>>>>>>> 12ccd66edd073d25e8b772a3558b1fbe4a9ded72

//	int first_half = floor((double) NUM_ELEMENTS / 2); 
//	int second_half = NUM_ELEMENTS - first_half; 

//	srand(0); 

	if(rank == 0) {
	
	printf("Please eneter how many processes you wanted to run at a time: ");
	scanf("%i", &num_procs);
	// intializing and printing
	printf("Unsorted:\t"); 
	init_array(data, NUM_ELEMENTS);
	print_array(data, NUM_ELEMENTS);
 	
	avg_data_per_process = size / num_procs; 
	
	
	// sends data to the other rank
	MPI_Send(&data[first_half], second_half, MPI_INT, 1, 0, MPI_COMM_WORLD); 
	
	// sorts the first half
	qsort(data, first_half, sizeof(int), compare_int); 

	// receive sorted second half of the data
	MPI_Recv(&data[first_half], second_half, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

	// merging the two sorted arrays
	merge(data, first_half, &data[first_half], second_half);
	
	for(int an_id = 1; an_id < num_procs; an_id++) {
		starting_index = an_id * avg_data_per_process + 1;
		ending_index = (an_id + 1) * avg_data_per_process; 
	
		if((starting_index - ending_index) < avg_data_per_process) 
			ending_index = size - 1; 
		
		data_to_be_sent = ending_index - starting_index + 1;
		
		MPI_Send(&data[starting_index], ending_index, MPI_INT, an_id, data_tag, MPI_COMM_WORLD); 
		qsort(data, starting_index, sizeof(int), compare_int); 
		MPI_Recv(&data[starting_index], ending_index, MPI_INT, an_id, data_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  
		
		merge(data, starting_index, &data[starting_index], ending_index);		
		data_tag++; 

	
	}	

	
	// printing sorted array
	printf("Sorted:\t\t"); 
	print_array(data, NUM_ELEMENTS);	
	
	} else if (rank == 1) {
		
	// receives half of the data 
//	MPI_Recv(data, second_half, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	// sort the received data 
//	qsort(data, second_half, sizeof(int), compare_int); 
	
	// send back sorted data 
//	MPI_Send(data, second_half, MPI_INT, 0, 0, MPI_COMM_WORLD);	
 	}

MPI_Finalize();

return 0;

<<<<<<< HEAD
}*/
=======
} */ 
>>>>>>> 12ccd66edd073d25e8b772a3558b1fbe4a9ded72
