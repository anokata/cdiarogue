build: 
	gcc solution.c -o solution
sdltest: 
	gcc sdltest.c -lSDL -lSDL_ttf -lSDL_gfx `pkg-config --cflags --libs glib-2.0` -o sdltest && ./sdltest
bf: 
	gcc bf.c -lcurses && ./a.out
sdltestdebug: 
	gcc sdltest.c -g -lSDL -lSDL_ttf -lSDL_gfx `pkg-config --cflags --libs glib-2.0` -o sdltest && ./sdltest

hashmap:
	gcc hashmap.c dlist.c && ./a.out

dlist:
	gcc dlist.c && ./a.out

dlistdebug:
	gcc -DDEBUG dlist.c && ./a.out
# TODO write 1 and 2 stderr to out file and open it in vim
