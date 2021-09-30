# For MPI programs 
CC = mpicc
OBJ = exercise-1
# flags
CFLAGS = -g -Wall -Werror 
LFLAGS = -lm -o $(OBJ)

$(OBJ): $(OBJ).c 
	$(CC) $(CFLAGS) $(OBJ).c $(LFLAGS) 


clean: 
	$(RM) -rf exercise-1 *.o *.~ *.dSYM 

