# MPI programs
CC = mpic++

#CC = g++

# flags
CFLAGS = -g -Wall -lm -O3 

all: main 

main: main.cpp 
	$(CC) $(CFLAGS) main.cpp -o main	

clean:
	$(RM) -rf main a_ans.csv b_ans.csv c_ans.csv d_ans.csv *.o *.~ *.dSYM  

run:
	./main
