# For MPI programs 
CC = mpicc
OBJ = exercise-1
# flags
CFLAGS = -g -Wall -Werror 

$(OBJ): $(OBJ).c 
	$(CC) $(CFLAGS) $(OBJ).c -o $(OBJ) 


clean: 
	$(RM) -rf exercise-1 *.o *.~ *.dSYM 

