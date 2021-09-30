# For MPI programs 
CC = mpicc
OBJ = 1
# flags
CFLAGS = -g -Wall -Werror 
LFLAGS = -lm -o $(OBJ)

$(OBJ): $(OBJ).c 
	$(CC) $(CFLAGS) $(OBJ).c $(LFLAGS) 


clean: 
	$(RM) -f $(OBJ) *.o *.~

