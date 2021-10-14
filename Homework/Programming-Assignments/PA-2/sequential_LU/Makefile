# For sequential LU decompostion

CC = gcc

# Flags
CFLAGS = -g -Wall -lm -O3

all: sequential_LU

sequential_LU: sequential.o main.o
	$(CC) $(CFLAGS) -o sequential_LU sequential.o main.o

main.o: main.c sequential.h 
	$(CC) $(CFLAGS) -c main.c

sequential.o: sequential.c 
	$(CC) $(CFLAGS) -c sequential.c

clean: 
	$(RM) sequential_LU *.o *~

run: 
	./sequential_LU
