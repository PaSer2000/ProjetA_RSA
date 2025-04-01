CC = gcc
CFLAGS = -c -g -Wall

all: Executable

Executable: rsa_tools.o phase1.o bezout.o test.o
	$(CC) -o Executable rsa_tools.o phase1.o bezout.o test.o

rsa_tools.o: rsa_tools.c rsa_common_header.h bezout.h
	$(CC) $(CFLAGS) rsa_tools.c

phase1.o: phase1.c rsa_common_header.h test.h bezout.h 
	$(CC) $(CFLAGS) phase1.c

bezout.o: bezout.c bezout.h
	$(CC) $(CFLAGS) bezout.c

test.o: test.c rsa_common_header.h test.h bezout.h
	$(CC) $(CFLAGS) test.c

other_base64.o: other_base64.c 
	$(CC) $(CFLAGS) other_base64.cc

clean:
	rm -f Executable *.o

