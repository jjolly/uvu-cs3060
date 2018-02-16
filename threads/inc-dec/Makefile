CFLAGS := $(CFLAGS) -g -pthread

all: $(patsubst %.c,%,$(wildcard *.c))

%:: %.c Makefile
	gcc $(CFLAGS) -o $@ $@.c

.PHONY: all
