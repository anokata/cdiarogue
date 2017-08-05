#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> 
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
int proc_dir(char* fn, int(*fun)(char*, char*), char* cur_pid);

int count_childrens(char* content, char* cur_pid) {
    int count = 0;
    char* ppid = get_ppid_from_stat(content);
    if (strcmp(ppid, cur_pid) == 0) {
        count++;
        if (strlen(ppid) > 1) {
            char* pid = get_pid_from_stat(content);
            int child_count = proc_dir("stat", count_childrens, pid);
            count += child_count;
        }
    }
    return count;
}

int proc_dir(char* fn, int(*fun)(char*, char*), char* cur_pid) {
    int res = 0;
    static char path[] = "/proc/";
    char filename[256];
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir: ");
        return 0;
    }
    struct dirent* cur;
    cur = readdir(dir);
    while (cur) {
        if (is_digit_name(cur->d_name)) {
            sprintf(filename, "/proc/%s/%s", cur->d_name, fn);
            char* content = read_onestr_file(filename);
            if (content) {
                res += fun(content, cur_pid);
            }
        }
        cur = readdir(dir);
    }
    if (closedir(dir)) {
        perror("closedir: ");
        return 0;
    }
    return res;
}

int main(int argc, char** argv) {
    int count = 0;
    if (argc < 2) return 0;
    count = proc_dir("stat", count_childrens, argv[1]);
    printf("%d\n", ++count);
    return 0;
}

