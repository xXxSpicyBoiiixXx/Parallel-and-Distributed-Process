nt main(int argc, char** argv) {
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
