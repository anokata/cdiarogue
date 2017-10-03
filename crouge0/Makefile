# vim: filetype=make
OBJECTS=app.c main.c map_gen.c world_map.c g.c tile_map.c util.c config_parser.c actor.c render.c collide.c events.c behavior.c item.c inventory.c charinfo.c object.c
MAP_OBJECTS=test_mapgen.c map_gen.c tile_map.c util.c config_parser.c actor.c item.c
TMAP_OBJECTS=tile_map.c test_tile_map.c util.c

BUILD_DIR=/tmp/build

P=rouge0
MAPGEN=test_mapgen
LIB=libccurses.a
CC=gcc
LFLAGS=-L../lib -lccurses -lncurses 
IFLAGS=-I./include -I../ccurses
GLIB=$(shell pkg-config --cflags --libs glib-2.0)
DEBUG=-DDEBUG
WORK=$(shell if [ -e ~/.work.sig ]; then echo -DWORK; fi)
# TODO Debug flag
CFLAGS=$(DEBUG) $(WORK) -Wall -Wextra -std=gnu11 -g -D_GNU_SOURCE=1 $(IFLAGS) $(LFLAGS) $(GLIB)

# Must be first rule, for compile main app
main: prepare
	$(eval DEBUG=)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(BUILD_DIR)/$(P) && $(BUILD_DIR)/$(P)

debug: prepare
	$(CC) $(OBJECTS) $(CFLAGS) -o $(BUILD_DIR)/$(P) && $(BUILD_DIR)/$(P)

prepare:
	if [ ! -e ../lib ]; then mkdir ../lib; fi
	if [ ! -e ../lib/$(LIB) ]; then cd ../ccurses; make makelib; cd -; fi
	mkdir $(BUILD_DIR) &> /dev/null || true

lint:
	splint main.c $(IFLAGS)

test_mapgen: prepare
	$(CC) $(MAP_OBJECTS) $(CFLAGS) -o $(BUILD_DIR)/$@ && valgrind $(BUILD_DIR)/$@ && ls -l $(BUILD_DIR)/*.map

test_sky: prepare
	$(CC) $(CFLAGS) test_sky.c sky_view.c $(GLIB) -I./include -I../ccurses -o $(BUILD_DIR)/$@ && $(BUILD_DIR)/$@

# TODO make build *.o to BUILD_DIR and *.c depened and target dep
test_tile_map: prepare
	$(CC) test_tile_map.c config_parser.c tile_map.c item.c object.c actor.c util.c $(CFLAGS) -o $(BUILD_DIR)/$@ && valgrind $(BUILD_DIR)/$@

test_config_parser: prepare
	$(eval DEBUG=-DDEBUG)
	$(CC) test_config_parser.c config_parser.c util.c $(CFLAGS) -o $(BUILD_DIR)/$@ && valgrind $(BUILD_DIR)/$@

test_events: prepare
	$(CC) $@.c actor.c events.c util.c tile_map.c config_parser.c g.c world_map.c map_gen.c  $(CFLAGS) -o $(BUILD_DIR)/$@ && valgrind $(BUILD_DIR)/$@

test_item: prepare
	$(CC) $@.c item.c actor.c events.c util.c tile_map.c config_parser.c g.c world_map.c map_gen.c  $(CFLAGS) -o $(BUILD_DIR)/$@ && valgrind $(BUILD_DIR)/$@

test_actor: prepare
	$(CC) $@.c item.c actor.c events.c util.c tile_map.c config_parser.c g.c world_map.c map_gen.c  $(CFLAGS) -o $(BUILD_DIR)/$@ && valgrind $(BUILD_DIR)/$@

test_object: prepare
	$(CC) $@.c object.c item.c actor.c events.c util.c tile_map.c config_parser.c g.c world_map.c map_gen.c  $(CFLAGS) -o $(BUILD_DIR)/$@ && valgrind $(BUILD_DIR)/$@

val:
	valgrind $(BUILD_DIR)/$(P)

mapview:
	cd tools
	make $@

toolmodule:
	if [ -z $(name) ]; then exit; fi
	@echo Creating $(name) module
	mkdir $(name)
	touch $(name)/$(name).c
	mkdir $(name)/include
	touch $(name)/include/$(name).h
	echo '#pragma once' >> $(name)/include/$(name).h
	echo '#include "$(name).h"' >> $(name)/$(name).c
	@echo successfully created.

module:
	# using make module name=somename
	@echo Creating $(name) module
	@echo '#pragma once' >> include/$(name).h
	@echo '#include "$(name).h"' >> $(name).c
	# create test
	@echo Create test
	@echo '#include "$(name).h"' >> test_$(name).c
	@echo '#include <stdio.h>'   >> test_$(name).c
	@echo '#include <assert.h>'  >> test_$(name).c
	@echo -e '\nint main() {\n\n}'  >> test_$(name).c
	@echo successfully created.

coverage:
	# using gcov and lcov !
	$(eval CFLAGS=$(CFLAGS) -fprofile-arcs -ftest-coverage -O0)
	$(eval DEBUG=)
	@echo "$(tput setaf 0; tput setab 7;)>>> Copy sources$(tput sgr 0;)"
	cp -R . $(BUILD_DIR)/src
	@echo ">>> Copy lib souces"
	cp -R ../ccurses $(BUILD_DIR)/ccurses
	mkdir $(BUILD_DIR)/lib || true
	@echo ">>> build lib"
	(cd $(BUILD_DIR)/ccurses; make makelib)
	@echo ">>> build app"
	(cd $(BUILD_DIR)/src; GCOV_PREFIX=$(BUILD_DIR) $(CC) $(OBJECTS) $(CFLAGS) -o $(BUILD_DIR)/$(P))
	@echo ">>> run app"
	$(BUILD_DIR)/$(P)
	(cd $(BUILD_DIR)/src; gcov *.gcda)
	# gcov main.gcno
	# https://habrahabr.ru/post/244835/

