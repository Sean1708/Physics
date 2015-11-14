CFLAGS = -Weverything --std=c11

debug: CFLAGS += -g -DDEBUG -Werror -Wno-unused-parameter -O0
debug: all

release: CFLAGS += -DNDEBUG -O3
release: all

all: openmp

openmp: openmp.c

.PHONY: clean
clean:
	rm -rf openmp openmp.dSYM
