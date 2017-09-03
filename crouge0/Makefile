# vim: filetype=make
OBJECTS=app.c main.c map_gen.c world_map.c g.c tile_map.c util.c config_parser.c actor.c render.c collide.c events.c
MAP_OBJECTS=mapgen_main.c map_gen.c tile_map.c util.c config_parser.c
TMAP_OBJECTS=tile_map.c test_tile_map.c util.c

BUILD_DIR=/tmp/build

P=rouge0
MAPGEN=mapgen_main
LIB=libccurses.a
CC=gcc
LFLAGS=-L../lib -lccurses -lncurses 
IFLAGS=-I./include -I../ccurses
GLIB=$(shell pkg-config --cflags --libs glib-2.0)
DEBUG=-DDEBUG
WORK=-DWORK_
CFLAGS=$(DEBUG) $(WORK) -Wall -Wextra -std=gnu11 -g -D_GNU_SOURCE=1 $(IFLAGS) $(LFLAGS) $(GLIB)

# Must be first rule, for compile main app
main: prepare
	$(eval DEBUG=)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(BUILD_DIR)/$(P) && $(BUILD_DIR)/$(P)

prepare:
	if [ ! -e ../lib ]; then mkdir ../lib; fi
	if [ ! -e ../lib/$(LIB) ]; then cd ../ccurses; make makelib; cd -; fi
	mkdir $(BUILD_DIR) 2&> /dev/null || true

lint:
	splint main.c $(IFLAGS)

mapgen_main: prepare
	$(CC) $(MAP_OBJECTS) $(CFLAGS) -o $(BUILD_DIR)/$@ && valgrind $(BUILD_DIR)/$@ && ls -l $(BUILD_DIR)/*.map

test_sky: prepare
	$(CC) $(CFLAGS) test_sky.c sky_view.c $(GLIB) -I./include -I../ccurses -o $(BUILD_DIR)/$@ && $(BUILD_DIR)/$@

# TODO make build *.o to BUILD_DIR and *.c depened and target dep
test_tile_map: prepare
	$(CC) test_tile_map.c config_parser.c tile_map.c util.c $(CFLAGS) -o $(BUILD_DIR)/$@ && valgrind $(BUILD_DIR)/$@

test_config_parser: prepare
	$(CC) test_config_parser.c config_parser.c util.c $(CFLAGS) -o $(BUILD_DIR)/$@ && valgrind $(BUILD_DIR)/$@

test_events: prepare
	$(CC) $@.c actor.c events.c util.c tile_map.c config_parser.c g.c world_map.c map_gen.c  $(CFLAGS) -o $(BUILD_DIR)/$@ && valgrind $(BUILD_DIR)/$@

val:
	valgrind $(BUILD_DIR)/$(P)

mapview:
	cd tools
	make $@

module:
	if [ -z $(name) ]; then exit; fi
	@echo Creating $(name) module
	mkdir $(name)
	touch $(name)/$(name).c
	mkdir $(name)/include
	touch $(name)/include/$(name).h
	echo '#pragma once' >> $(name)/include/$(name).h
	echo '#include "$(name).h"' >> $(name)/$(name).c
	@echo successfully created.

