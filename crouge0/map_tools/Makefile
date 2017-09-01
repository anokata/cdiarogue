# vim: filetype=make
P=mapview
LFLAGS=-L../lib -L../../lib -lccurses -lncurses 
IFLAGS=-I./include -I../../ccurses -I../include

# TODO include it
BUILD_DIR=/tmp/build
OBJECTS=*.c ../tile_map.c ../util.c ../config_parser.c
CC=gcc
GLIB=$(shell pkg-config --cflags --libs glib-2.0)
CFLAGS=-Wall -Wextra -std=gnu11 -g -D_GNU_SOURCE=1 $(IFLAGS) $(LFLAGS) $(GLIB)

map_view:
	$(CC) $(OBJECTS) $(CFLAGS) -o $(BUILD_DIR)/$@ && valgrind $(BUILD_DIR)/$@
