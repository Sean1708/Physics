CFLAGS = -Weverything -Os

debug: CFLAGS += -g -DDEBUG -Werror -Wno-unused-parameter
debug: release

release: array

array: array.c

.PHONY: clean
clean:
	rm -rf array array.dSYM
