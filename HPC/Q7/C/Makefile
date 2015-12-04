CFLAGS = -Weverything --std=c11 -O0 -g -Wno-reserved-id-macro

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

.PHONY: run
run:
	mpirun heatbath-parallel

.PHONY: clean
clean:
	rm -rf heatbath-serial heatbath-parallel
	rm -rf *.dSYM *.o
