# For MPI programs 
CC = mpicc

# flags
CFLAGS = -g -Wall -Wextra -Werror

all: exercise1

exercise1: exercise1.o
	$(CC) $(CFLAGS) -o 1 exercise1.o

exercise1.o: exercise1.c
	$(CC) $(CFLAGS) -c exercise1.c


clean: 
	$(RM) 1 *.o *.~

run: 
	mpiexec -n 2 ./1
