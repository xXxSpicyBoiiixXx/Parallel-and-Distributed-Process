# OpenMP version of LU decomposition 

CC = gcc

# Flags 

CFLAGS = -g -Wall -Wextra -lm -O3

all: openmp_LU

openmp_LU: main.o
	$(CC) $(CFLAGS) -o openmp_LU -fopenmp main.o

main.o: main.c
	$(CC) $(CFGLAGS) -c -fopenmp main.c 

clean:
	$(RM) openmp_LU *.o *~

run:
	./openmp_LU
