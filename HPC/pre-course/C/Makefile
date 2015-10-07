CFLAGS = -Weverything -Os

debug: CFLAGS += -g -DDEBUG -Werror -Wno-unused-parameter
debug: release

release: pi

pi: pi.c

.PHONY: clean
clean:
	rm -rf pi pi.dSYM
