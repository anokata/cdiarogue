# vim: filetype=make
P=todonotes
CFLAGS=-Wall -Wextra -std=gnu11 -g -D_GNU_SOURCE=1
INCLUDES=-I./include -I../include
OBJECTS=main.c note.c
CC=gcc
TARGET=/tmp/$(P)
TEMPOUT=-o $(TARGET)
RUN=$(TARGET)

main: 
	$(CC) $(CFLAGS) $(OBJECTS) $(INCLUDES) $(TEMPOUT) && $(RUN)

run:
	$(RUN)

compile:
	$(CC) $(CFLAGS) $(OBJECTS) $(INCLUDES) $(TEMPOUT)

test:
	valgrind $(RUN)

.PHONY=main run compile test
