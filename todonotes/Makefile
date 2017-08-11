# vim: filetype=make
P=todonotes
CFLAGS=-Wall -Wextra -std=gnu11 -g -D_GNU_SOURCE=1
INCLUDES=-I./include -I../include
PKG=$(shell pkg-config --libs --cflags glib-2.0)
OBJECTS=main.c note.c notesbook.c
CC=gcc
TARGET=/tmp/$(P)
TEMPOUT=-o $(TARGET)
RUN=valgrind $(TARGET)

main: 
	$(CC) $(CFLAGS) $(OBJECTS) $(INCLUDES) $(PKG) $(TEMPOUT) && $(RUN)

run:
	$(RUN)

compile:
	$(CC) $(CFLAGS) $(OBJECTS) $(INCLUDES) $(TEMPOUT)

test:
	valgrind $(RUN)

x: a b 
	@echo $(wildcard *.c)
	@echo $(wildcard */*.c)
	@echo $(addprefix ./, a b d)
	@echo $(addprefix ./, $(OBJECTS))
	@echo $@
	@echo $<
	@echo $^

	#include gcc *.c -MM $(INCLUDES)
a:
b:

.PHONY=main run compile test
