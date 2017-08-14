# vim: filetype=make
P=time
CFLAGS=-Wall -Wextra -std=gnu11 -g -D_GNU_SOURCE=1
OBJECTS=time.c
CC=gcc
TARGET=/tmp/$(P)
TEMPOUT=-o $(TARGET)
RUN=$(TARGET)

main: 
	$(CC) $(CFLAGS) $(OBJECTS) $(TEMPOUT) && $(RUN)
