#pragma once
#ifndef __UTIL__
#define __UTIL__
#include <stdio.h>
#include <stdlib.h>
typedef char* string;

#define UNUSED(x) ((void)(x));

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)
#endif

int fget_int_line(FILE *file);

#endif
