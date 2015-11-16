CC = mpicc
CFLAGS = -O0 -g -Wall -Wextra -pedantic -std=c99

all: hello ping

hello: hello.o

ping: ping.o

.PHONY: clean
clean:
	rm -rf hello
	rm -rf *.dSYM *.o run_mpi.sh.*
