CC = gcc
CFLAGS = -c -g -Wall -lgmp
LDFLAGS = -lgmp

#Cibles principales
all: Executable

#Cibles spécifiques à chaque phase
phase1: phase1_exec
phase2: phase2_exec
phase 3: phase3_exec

#Cibles spécifiques
phase1_exec: rsa_tools.o phase1.o bezout.o test.o int2char.o
	$(CC) -o phase1_exec rsa_tools.o phase1.o bezout.o test.o int2char.o $(LDFLAGS)

phase2_exec: rsa_tools.o phase2.o bezout.o test.o int2char.o
	$(CC) -o phase2_exec rsa_tools.o phase2.o bezout.o test.o int2char.o $(LDFLAGS)

phase3_exec : rsa_tools.o phase3.o bezout.o int2char.o sha256.o
	$(CC) -o phase3_exec rsa_tools.o phase3.o bezout.o int2char.o sha256.o $(LDFLAGS)

Executable: rsa_tools.o phase1.o bezout.o test.o
	$(CC) -o Executable rsa_tools.o phase1.o bezout.o test.o $(LDFLAGS)

#les .o
rsa_tools.o: rsa_tools.c rsa_common_header.h bezout.h int2char.h
	$(CC) $(CFLAGS) rsa_tools.c

phase1.o: phase1.c rsa_common_header.h test.h bezout.h
	$(CC) $(CFLAGS) phase1.c

phase2.o: phase2.c rsa_common_header.h test.h bezout.h
	$(CC) $(CFLAGS) phase2.c

phase3.o: phase3.c rsa_common_header.h 
	$(CC) $(CFLAGS) phase3.c

bezout.o: bezout.c bezout.h
	$(CC) $(CFLAGS) bezout.c

test.o: test.c rsa_common_header.h test.h bezout.h
	$(CC) $(CFLAGS) test.c

int2char.o: int2char.c int2char.h 
	$(CC) $(CFLAGS) int2char.c

sha256.o : sha256.c sha256.h
	$(CC) $(CFLAGS) sha256.c
#clean
clean:
	rm -f Executable phase1_exec phase2_exec *.o
