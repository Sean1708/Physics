CFLAGS = -Wall -Wextra -pedantic -std=c99 -O3

.PHONY: all
all: mpi serial

.PHONY: mpi
mpi: CC = mpicc
mpi: heatbath-parallel

.PHONY: serial
serial: heatbath-serial

heatbath-parallel: heatbath-parallel.o grid.o

heatbath-serial: heatbath-serial.o grid.o

grid.o: grid.c grid.h


.PHONY: clean
clean:
	rm -rf heatbath-serial heatbath-parallel
	rm -rf *.dSYM *.o
