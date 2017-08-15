#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

char log_filename[] = "/tmp/clog";

void debug_file_log(char *msg) {
    FILE *fout = fopen(log_filename, "a+");
    fwrite(msg, strlen(msg), 1, fout);
    fclose(fout);
}

void debug_file_log_format(const char* format, ...) {
    FILE *fout = fopen(log_filename, "a+");
    va_list argptr;
    va_start(argptr, format);
    vfprintf(fout, format, argptr);
    va_end(argptr);
    fclose(fout);
}
