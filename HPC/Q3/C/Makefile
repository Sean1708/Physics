CFLAGS = -Weverything -Os

debug: CFLAGS += -g -DDEBUG -Werror -Wno-unused-parameter
debug: release

release: floats

floats: floats.c

.PHONY: clean
clean:
	rm -rf floats floats.dSYM
