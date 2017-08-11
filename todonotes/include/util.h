#pragma once
#include <stdio.h>
#include <stdlib.h>
typedef char* string;

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)
#endif


