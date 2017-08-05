#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "mylib.h"

#define BUFSIZE 30

char* get_ppid(char* pid) {
    char proc_path[BUFSIZE];
    char *ppid_str = "PPid";
    sprintf(proc_path, "/proc/%s/status", pid);

    FILE *f = fopen(proc_path, "r");
    if (!f) {
        perror("Open: ");
        return NULL;
    }
    char *pos = 0;
    char str[BUFSIZE+1];
    char *res = str;

    while (!pos && res) {
        res = fgets(str, BUFSIZE, f);
        pos = strstr(str, ppid_str);
    }
    if (pos == 0) return NULL;
    if (fclose(f)) {
        perror("Close: ");
        return NULL;
    }
    static char ppid_s[BUFSIZE];
    int k = 0;
    int len = strlen(str);
    int i = 0;
    for (i = 0; i < len; i++) {
        if (isdigit(str[i])) {
            ppid_s[k++] = str[i];
        }
    }
    ppid_s[k++] = '\0';

    return ppid_s;
}

int ppid_tree(char* start_pid) {
    printf("%s\n", start_pid);
    char* pids = get_ppid(start_pid);
    while (strcmp(pids, "1") != 0) {
        printf("%s\n", pids);
        pids = get_ppid_s(pids);
    }
    printf("%s\n", pids);
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) return 0;
    ppid_tree(argv[1]);
    return 0;
}
