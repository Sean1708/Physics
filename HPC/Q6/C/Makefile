CFLAGS = -Weverything --std=c11

debug: CFLAGS += -g -DDEBUG -Werror -Wno-unused-parameter -O0
debug: all

release: CFLAGS += -DNDEBUG -O3
release: all

jorvik: openmp.c
	gcc -Wall -Wextra -pedantic -O0 -std=c99  openmp.c   -o openmp   -lm -fopenmp

all: openmp

openmp: openmp.c

.PHONY: clean
clean:
	rm -rf openmp openmp.dSYM
