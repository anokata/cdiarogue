# vim: filetype=make
P=test
CFLAGS=-Wall -Wextra -std=gnu11 -g -D_GNU_SOURCE=1 
INCLUDES=-I./include 
PKG=$(shell pkg-config --libs --cflags glib-2.0)
OBJECTS=test_note.c ../note.c
CC=gcc
TARGET=/tmp/$(P)
TEMPOUT=-o $(TARGET)
RUN=$(TARGET)

main: 
	$(CC) $(CFLAGS) -I../include $(OBJECTS) $(TEMPOUT) && $(RUN)

run:
	$(RUN)

test:
	valgrind $(RUN)

test_note:
	$(CC) $(CFLAGS)  -I../include test_note.c ../note.c $(PKG) $(TEMPOUT) && valgrind $(TARGET)

test_notesbook:
	$(CC) $(CFLAGS)  -I../include test_notesbook.c ../notesbook.c ../note.c $(PKG) $(TEMPOUT) && valgrind $(TARGET)

.PHONY=main run test test_notesbook

