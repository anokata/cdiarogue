# vim: filetype=make
P=tn
CFLAGS=-Wall -Wextra -std=gnu11 -g -D_GNU_SOURCE=1
INCLUDES=-I./include -I../include
PKG=$(shell pkg-config --libs --cflags glib-2.0)
OBJECTS=main.c note.c notesbook.c command.c
CC=gcc
TARGET=/tmp/$(P)
TEMPOUT=-o $(TARGET)
RUN=valgrind --leak-check=full $(TARGET)
# VPATH := 

main: 
	$(CC) $(CFLAGS) $(OBJECTS) $(INCLUDES) $(PKG) $(TEMPOUT) && $(RUN)

run:
	$(RUN)

compile:
	$(CC) $(CFLAGS) $(OBJECTS) $(INCLUDES) $(TEMPOUT)

release:
	$(CC) -std=gnu11 -O2 -D_GNU_SOURCE=1 $(OBJECTS) $(INCLUDES) $(PKG) $(TEMPOUT) && $(RUN)
	strip $(TARGET)
	ls -lah $(TARGET)

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
