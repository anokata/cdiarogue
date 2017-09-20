# vim: filetype=make
P=main
CFLAGS=-Wall -Wextra -std=c11 -g 
OBJECTS=main.c
CC=gcc
TARGET=/tmp/$(P)
TEMPOUT=-o $(TARGET)
RUN=$(TARGET)

main: 
	$(CC) $(CFLAGS) $(OBJECTS) $(TEMPOUT) && $(RUN)
