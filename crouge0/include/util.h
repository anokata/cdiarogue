#pragma once
#ifndef __UTIL__
#define __UTIL__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef char* string;

#define UNUSED(x) ((void)(x));

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)
#endif

int fget_int_line(FILE *file);

typedef void (*EachStrFunc)(char* str, void *data);
void for_every_part(char *text, char delim, EachStrFunc f, void *data);

char *read_whole_file(char *filename);

#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))


#endif
