LDFLAGS = -lblas


macbook: CFLAGS += -Weverything --std=c11 -O3
macbook: LDFLAGS ?= -L/usr/local/opt/openblas/lib
macbook: CPPFLAGS ?= -I/usr/local/opt/openblas/include
macbook: all

jorvik: CFLAGS += -Wall -Wextra -pedantic -std=c99 -O3
jorvik: all

all: matmul

matmul: matmul.o matrix.o
	$(CC) matmul.o matrix.o $(LDFLAGS) -o matmul


.PHONY: clean
clean:
	rm -rf matmul matmul.dSYM
	rm -rf *.o
