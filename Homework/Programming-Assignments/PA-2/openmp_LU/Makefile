# OpenMP version of LU decomposition 

CC = gcc

# Flags 
CFLAGS = -g -Wall -Wextra -lm -O3

all: openmp_LU

openmp_LU: openmp.o main.o
	$(CC) $(CFLAGS) -o openmp_LU -fopenmp openmp.o main.o

main.o: main.c openmp.c
	$(CC) $(CFLAGS) -c -fopenmp main.c

openmp.o: openmp.c
	$(CC) $(CFLAGS) -c -fopenmp openmp.c

clean:
	$(RM) openmp_LU *.o *~

run: 
	./openmp_LU
