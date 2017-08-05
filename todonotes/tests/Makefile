# vim: filetype=make
P=test
CFLAGS=-Wall -Wextra -std=gnu11 -g -D_GNU_SOURCE=1
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

.PHONY=main run test

