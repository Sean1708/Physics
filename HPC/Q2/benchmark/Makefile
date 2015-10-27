CC = gcc
CFLAGS = -O2

FF = gfortran
FFLAGS = -O2

all: fort_stream c_stream

fort_stream: stream.f mysecond.o
	$(CC) $(CFLAGS) -c mysecond.c
	$(FF) $(FFLAGS) -c stream.f
	$(FF) $(FFLAGS) stream.o mysecond.o -o fort_stream

c_stream: stream.c
	$(CC) $(CFLAGS) stream.c -o c_stream

clean:
	rm -f fort_stream c_stream *.o

tar:
	tar -cvf stream.tar stream.f stream.c mysecond.c READ.ME Makefile
