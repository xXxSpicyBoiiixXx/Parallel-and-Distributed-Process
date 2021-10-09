# C program (NOT FOR MPI RELATED ITEMS) 
CC = gcc

OBJ1 = LU_rosettacode

#flags (what the hell is pendatic?
# removed flag "pendatic", seems to be legacy
CFLAGS = -std=gnu99 -Wall -lm -O3  

all: $(OBJ1) 

$(OBJ1): $(OBJ1).c 
	$(CC) $(CFLAGS) $(OBJ1).c -o $(OBJ1)

clean: 
	$(RM) -rf $(OBJ1) *.o 

