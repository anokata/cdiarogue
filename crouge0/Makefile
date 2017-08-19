# vim: filetype=make
P=rouge0
MAPGEN=mapgen_main
CFLAGS=-Wall -Wextra -std=gnu11 -g -D_GNU_SOURCE=1
OBJECTS=app.c main.c map_gen.c world_map.c g.c tile_map.c util.c config_parser.c
MAP_OBJECTS=mapgen_main.c map_gen.c tile_map.c util.c config_parser.c
TMAP_OBJECTS=tile_map.c test_tile_map.c util.c
LDLIBS=
LIB=libccurses.a
CC=gcc
GLIB=$(shell pkg-config --cflags --libs glib-2.0)

main:
	if [ ! -e ../lib ]; then mkdir ../lib; fi
	if [ ! -e ../lib/$(LIB) ]; then cd ../ccurses; make makelib; cd -; fi
	$(CC) $(CFLAGS) $(OBJECTS) -L../lib -lccurses -I./include -I../ccurses -lncurses $(GLIB) -o /tmp/$(P) && /tmp/$(P)

compile:
	$(CC) $(CFLAGS) *.c -L../lib -lccurses -I../ccurses -lncurses -o /tmp/$(P)

makelib:
	$(CC) -std=c11 lib/*.c -c
	ar rvs libccurses.a *.o
	rm *.o
	# make to temp

make_with_lib:
	$(CC) -Wall -std=c11 *.c -L../lib -lccurses -I../ccurses -lncurses -o /tmp/a.out && /tmp/a.out


mapgen_main:
	$(CC) $(CFLAGS) $(MAP_OBJECTS) -L../lib -lccurses -I./include -I../ccurses -lncurses $(GLIB) -o /tmp/$(MAPGEN) && valgrind /tmp/$(MAPGEN) && ls -l /tmp/*.map

lint:
	splint main.c -I./include -I../ccurses/

test_sky:
	$(CC) $(CFLAGS) test_sky.c sky_view.c $(GLIB) -I./include -I../ccurses -o /tmp/sky && /tmp/sky

test_tile_map:
	$(CC) $(CFLAGS) test_tile_map.c config_parser.c tile_map.c util.c $(GLIB) -I./include -I../ccurses -lncurses -o /tmp/tiles && valgrind /tmp/tiles

test_config_parser:
	$(CC) $(CFLAGS) test_config_parser.c config_parser.c util.c $(GLIB) -I./include -I../ccurses -o /tmp/parser && valgrind /tmp/parser

