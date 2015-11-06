CFLAGS = -lblas

macbook: CFLAGS += -Weverything --std=c11 -O3
macbook: LDFLAGS ?= -L/usr/local/opt/openblas/lib
macbook: CPPFLAGS ?= -I/usr/local/opt/openblas/include
macbook: all

jorvik: CFLAGS += -Wall -Wextra -pedantic -std=c11 -O3
jorvik: all

all: matmul

matmul: matmul.c matrix.c

.PHONY: clean
clean:
	rm -rf matmul matmul.dSYM
