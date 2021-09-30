# MPI programs 
CC = mpicc
OBJ1 = exercise-1
OBJ2 = exercise-2

# flags
CFLAGS = -g -Wall -Werror 

all: $(OBJ1) $(OBJ2)

$(OBJ1): $(OBJ1).c
	$(CC) $(CFLAGS) $(OBJ1).c -o $(OBJ1)

$(OBJ2): $(OBJ2).c
	$(CC) $(CFLAGS) $(OBJ2).c -o $(OBJ2) 

clean:
	$(RM) -rf exercise-1 exercise-2 *.o *.~ *.dSYM

