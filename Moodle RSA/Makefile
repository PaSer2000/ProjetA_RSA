OBJ = bezout.o test.o phase1.o rsa_tools.o
SRC = bezout.c test.c phase1.c rsa_tools.c

all: Executable

%.o: %.c
	gcc -c $< -o $@

Executable: $(OBJ)
	gcc -o Executable $(OBJ)

clean:
	rm -f Executable $(OBJ)
