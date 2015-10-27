CFLAGS = -Weverything -Os

debug: CFLAGS += -g -DDEBUG -Werror -Wno-unused-parameter
debug: release

release: stream

stream: stream.c

.PHONY: clean
clean:
	rm -rf stream stream.dSYM
