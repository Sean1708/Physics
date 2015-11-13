CFLAGS = -Weverything --std=c11 -lblas
LDFLAGS ?= -L/usr/local/opt/openblas/lib
CPPFLAGS ?= -I/usr/local/opt/openblas/include

debug: CFLAGS += -g -DDEBUG -Werror -Wno-unused-parameter -O0
debug: all

release: CFLAGS += -DNDEBUG -O3
release: all

all: matmul

matmul: matmul.c matrix.c

.PHONY: clean
clean:
	rm -rf matmul matmul.dSYM
