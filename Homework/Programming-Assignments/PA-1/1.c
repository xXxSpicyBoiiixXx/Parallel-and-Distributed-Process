/*
 * FILE:
 * USAGE:
 * DESCRIPTION: 
 * OPTIONS:
 * REQUIREMENTS: 
 * BUGS: 
 * AUTHOR: xXxSpicyBoiiixXx
 * ORGANIZATION: Illinois Institute of Technology
 * VERSION: 1.4 
 * CREATED: 09/20/2021
 * REVISION: 
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <limits.h> 

/*
 * Function: powerOfTwo, this function checks if the number of process are a power of two. 
 * 	     if not then an error message will appear on the console. 
 * 	     
 * Input: int id, an interger that represents the current rank of the current process 
 * 	  int numberProcs, an interger with the total number of processes
 */
static void powerOfTwo(int id, int numberProcesses) {
	int power;

	power = (numberProcesses != 0) && ((numberProcesses & (numberProcesses - 1)) == 0);
	
	if (!power) {
		if (id == 0) {  
			printf("Number of processes must be power of 2 \n"); 
		} 
		
		MPI_Finalize();
		exit(-1);
	}
}


/*
 * Function: getInput, this function gets input from the user to construct an array size and then broadcasts it. 
 * 
 * Input: int argc, argument count 
 * 	  char* argv[], points to arguement vector
 * 	  int id, current rank of the current process 
 * 	  int numProcs, number of processes 
 * 	  int* arraySize, points to array size 
 */
static void getInput(int argc, char* argv[], int id, int numProcs, int* arraySize){
    if (id == 0){
        if (id % 2 != 0){
			fprintf(stderr, "usage: mpirun -n <p> %s <size of array> \n", argv[0]);
            fflush(stderr);
            *arraySize = -1;
        } else if (argc != 2){
            fprintf(stderr, "usage: mpirun -n <p> %s <size of array> \n", argv[0]);
            fflush(stderr);
            *arraySize = -1;
        } else if ((atoi(argv[1])) % numProcs != 0) {
		    fprintf(stderr, "size of array must be divisible by number of processes \n");
            fflush(stderr);
            *arraySize = -1;
		} else {
            *arraySize = atoi(argv[1]);
        }
    }

    MPI_Bcast(arraySize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (*arraySize <= 0) {
        MPI_Finalize();
        exit(-1);
    }
}

/*
 * Function: initArray, this function populates an array with random numbers
 *
 * Input: int array[], an array of integers
 * 	  int arraySize, array size 
 * 	  int id, process id
 */
static inline void fillArray(int array[], int arraySize, int id) {
	for (int i = 0; i < arraySize; i++) {
		array[i] = rand() % 100; //INT_MAX
	}
}


static void printList(char arrayName[], int array[], int arraySize) {
    printf(arrayName);
    for (int i = 0; i < arraySize; i++) {
        printf(" %d", array[i]);
    }
    printf("\n");
}

/*
 * Function: compare, a compariosn function used in qsort function
 *
 * Input: comparison between two ints where either -1, 0, or 1 is 
 * 	  return depending on if the first int is less than, equal,
 * 	  or greater than the second.
 */
static int compare(const void* a_p, const void* b_p) {
   int a = *((int*)a_p);
   int b = *((int*)b_p);

   if (a < b)
      return -1;
   else if (a == b)
      return 0;
   else /* a > b */
      return 1;
}


static int* merge(int half1[], int half2[], int mergeResult[], int size){
    int ai, bi, ci;
    ai = bi = ci = 0;
    // integers remain in both arrays to compare
    while ((ai < size) && (bi < size)){
        if (half1[ai] <= half2[bi]){
			mergeResult[ci] = half1[ai];
			ai++;
		} else {
			mergeResult[ci] = half2[bi];
			bi++;
		}
			ci++;
	}
	// integers only remain in rightArray
	if (ai >= size){
        while (bi < size) {
			mergeResult[ci] = half2[bi];
			bi++; ci++;
		}
	}
	// integers only remain in localArray
	if (bi >= size){
		while (ai < size) {
			mergeResult[ci] = half1[ai];
			ai++; ci++;
		}
	}
	return mergeResult;
}

static int* mergeSort(int height, int id, int localArray[], int size, MPI_Comm comm, int globalArray[]){
    int parent, rightChild, myHeight;
    int *half1, *half2, *mergeResult;

    myHeight = 0;
    qsort(localArray, size, sizeof(int), compare); // sort local array
    half1 = localArray;  // assign half1 to localArray
	
    while (myHeight < height) { // not yet at top
        parent = (id & (~(1 << myHeight)));

        if (parent == id) { // left child
		    rightChild = (id | (1 << myHeight));

  		    // allocate memory and receive array of right child
  		    half2 = (int*) malloc (size * sizeof(int));
  		    MPI_Recv(half2, size, MPI_INT, rightChild, 0,
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  		    // allocate memory for result of merge
  		    mergeResult = (int*) malloc (size * 2 * sizeof(int));
  		    // merge half1 and half2 into mergeResult
  		    mergeResult = merge(half1, half2, mergeResult, size);
  		    // reassign half1 to merge result
            half1 = mergeResult;
			size = size * 2;  // double size
			
			free(half2); 
			mergeResult = NULL;

            myHeight++;

        } else { // right child
			  // send local array to parent
              MPI_Send(half1, size, MPI_INT, parent, 0, MPI_COMM_WORLD);
              if(myHeight != 0) free(half1);  
              myHeight = height;
        }
    }

    if(id == 0){
		globalArray = half1;   // reassign globalArray to half1
	}
	return globalArray;
}

int main(int argc, char** argv) {
    int numProcs, id, globalArraySize, localArraySize, height;
    int *localArray, *globalArray;
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
		printList("UNSORTED ARRAY:", globalArray, globalArraySize);  // Line A
	}
	
    // allocate memory for local array, scatter to fill with values and print
    localArraySize = globalArraySize / numProcs;
    localArray = (int*) malloc (localArraySize * sizeof(int));
    MPI_Scatter(globalArray, localArraySize, MPI_INT, localArray, 
		localArraySize, MPI_INT, 0, MPI_COMM_WORLD);
    
    //Merge sort
    if (id == 0) {
		globalArray = mergeSort(height, id, localArray, localArraySize, MPI_COMM_WORLD, globalArray);
	}
	else {
	        mergeSort(height, id, localArray, localArraySize, MPI_COMM_WORLD, NULL);
	}

    if (id == 0) {
		printList("FINAL SORTED ARRAY:", globalArray, globalArraySize);  // Line C
		free(globalArray);
	}

    free(localArray);  
    MPI_Finalize();
    return 0;
}

