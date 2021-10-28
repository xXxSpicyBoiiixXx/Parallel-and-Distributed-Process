# MPI programs
#CC = mpicc 

CC = g++

# flags
CFLAGS = -g -Wall -lm -O3 

all: main 

main: main.cpp 
	$(CC) $(CFLAGS) main.cpp -o main	

clean:
	$(RM) -rf main *.o *.~ *.dSYM  

run:
	./main
