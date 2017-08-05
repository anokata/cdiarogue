#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

char* read_onestr_file(char *filename);
char* get_ppid_s(char* pid);
int is_digit_name(char *str);
char* get_ppid_from_stat(char* stat);
char* get_pid_from_stat(char* stat);
