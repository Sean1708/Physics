CFLAGS = -Weverything --std=c11 -O0 -g -Wno-reserved-id-macro

.PHONY: mpi
mpi: CC = mpicc
mpi: heatbath-parallel

.PHONY: serial
serial: heatbath-serial

heatbath-parallel: heatbath-parallel.o matrix.o

heatbath-serial: heatbath-serial.o matrix.o

matrix.o: matrix.c matrix.h

.PHONY: run
run:
	mpirun heatbath-parallel

.PHONY: clean
clean:
	rm -rf heatbath-serial heatbath-parallel
	rm -rf *.dSYM *.o
