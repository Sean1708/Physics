CFLAGS = -Weverything -Os --std=c11

debug: CFLAGS += -g -DDEBUG -Werror -Wno-unused-parameter
debug: release

release: floats

floats: floats.c

.PHONY: clean
clean:
	rm -rf floats floats.dSYM
