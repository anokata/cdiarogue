LIB=libfurses.a
CC=gcc
CFLAGS=-Wall -std=gnu11 -g 
LIBFLAGS=-L../ccurses -lccurses -I./ -I./lib -lncurses 

main:
	$(CC) -Wall -std=c11 *.c lib/*.c -lncurses -o /tmp/a.out && /tmp/a.out

makelib:
	$(CC) -std=c11 lib/*.c -c
	ar rvs libccurses.a *.o
	rm *.o
	# make to temp

make_with_lib:
	$(CC) -Wall -std=c11 *.c -L../ccurses -lccurses -I../ccurses -lncurses -o /tmp/a.out && /tmp/a.out

state_test:
	$(CC) $(CFLAGS) $(LIBFLAGS) state_test.c lib/state.c -o /tmp/state && valgrind /tmp/state
