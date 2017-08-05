#include "mylib.h"
#define BUFSIZE 30

char* read_onestr_file(char *filename) { //mylib
    FILE *fd = fopen(filename, "r");
    if (!fd) {
        perror("open file: ");
        return NULL;
    }
    static char buf[256];
    char* res = fgets(buf, 255, fd);
    if (!res) {
        perror("read file: ");
        return NULL;
    }
    buf[strlen(buf) - 1] = '\0';
    if (fclose(fd) != 0) {
        perror("close file: ");
        return NULL;
    }
    return buf;
}

char* get_ppid_from_stat(char* stat) {
    char s1[BUFSIZE];
    char s2[BUFSIZE];
    char s3[BUFSIZE];
    static char ppid[BUFSIZE];
    sscanf(stat, "%s %s %s %s", s1, s2, s3, ppid);
    return ppid;
}

char* get_pid_from_stat(char* stat) {
    static char ppid[BUFSIZE];
    sscanf(stat, "%s", ppid);
    return ppid;
}

char* get_ppid_s(char* pid) { // mylib
    char proc_path[BUFSIZE];
    sprintf(proc_path, "/proc/%s/stat", pid);
    char* stat = read_onestr_file(proc_path);
    char* ppid = get_ppid_from_stat(stat);
    return ppid;
}

int is_digit_name(char *str) {
    while(*str) {
        if (isdigit(*str))
            return -1;
        str++;
    }
    return 0;
}
