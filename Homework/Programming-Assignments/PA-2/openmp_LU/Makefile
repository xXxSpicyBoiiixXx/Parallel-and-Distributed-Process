# OpenMP version of LU decomposition 

CC = gcc

# Flags 
CFLAGS = -g -Wall -lm -O3

all: openmp_LU

openmp_LU: openmp.o main.o
	$(CC) $(CFLAGS) -o openmp_LU -fopenmp openmp.o main.o

main.o: main.c openmp.c
	$(CC) $(CFLAGS) -c main.c

openmp.o: openmp.c
	$(CC) $(CFLAGS) -c openmp.c

clean:
	$(RM) openmp_LU *.o *~

run: 
	./openmp_LU
