CFLAGS = -Weverything --std=c11

debug: CFLAGS += -g -Werror -Wno-unused-parameter -O0
debug: all

release: CFLAGS += -DNDEBUG -O3
release: all

all: heatbath

heatbath: heatbath.o matrix.o

matrix.o: matrix.c matrix.h

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf heatbath heatbath.dSYM
