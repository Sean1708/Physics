CFLAGS = -Weverything --std=c11

debug: CFLAGS += -g -DDEBUG -Werror -Wno-unused-parameter
debug: all

release: CFLAGS += -Ofast

all: profile

profile: profile.c

.PHONY: clean
clean:
	rm -rf profile profile.dSYM
