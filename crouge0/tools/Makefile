# vim: filetype=make
P=mapview
LFLAGS=-L../lib -L../../lib -lccurses -lncurses 
IFLAGS=-I./include -I../../ccurses -I../include

# TODO include it
BUILD_DIR=/tmp/build
OBJECTS=../tile_map.c ../util.c ../config_parser.c ../item.c ../actor.c ../map_gen.c
CC=gcc
GLIB=$(shell pkg-config --cflags --libs glib-2.0)
CFLAGS=-Wall -Wextra -std=gnu11 -g -D_GNU_SOURCE=1 $(IFLAGS) $(LFLAGS) $(GLIB)
RUN=$(BUILD_DIR)/$@

prepare:
	mkdir -p $(BUILD_DIR) &> /dev/null || true

mapview: prepare
	$(CC) mapview.c $(OBJECTS) $(CFLAGS) -o $(BUILD_DIR)/$@ && $(RUN)

mapreplace: prepare
	$(CC) mapreplace.c $(OBJECTS) $(CFLAGS) -o $(BUILD_DIR)/$@ && $(RUN)

genmap: prepare
	$(CC) genmap.c $(OBJECTS) $(CFLAGS) -o $(BUILD_DIR)/$@ && $(RUN)
