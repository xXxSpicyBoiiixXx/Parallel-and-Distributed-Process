# For MPI programs 
CC = mpicc

# flags
CFLAGS = -g -Wall -Wextra -Werror

all: 1

1: 1.o
	$(CC) $(CFLAGS) -o 1 1.o

1.o: 1.c
	$(CC) $(CFLAGS) -c 1.c


clean: 
	$(RM) 1 *.o *.~

run: 
	mpiexec -n 2 ./1
