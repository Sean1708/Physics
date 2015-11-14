CFLAGS = -std=c11 -Wall -Wextra -pedantic -Wno-unused-parameter -fopenmp
LDFLAGS = -fopenmp

debug: CFLAGS += -Werror -g -O0
debug: all

release: CFLAGS += -DNDEBUG -O3
release: all

all: hello serial parallel

hello: hello.o

serial: serial.o

parallel: parallel.o

.PHONY: clean
clean:
	rm -rf hello serial parallel
	rm -rf *.dSYM
	rm -rf *.o
