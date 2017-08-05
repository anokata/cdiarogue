#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include "mylib.h"
#include <stdlib.h> 
#include <signal.h>

int proc_dir(char* fn, int(*fun)(char*, char*), char* cur_pid);

int count_childrens(char* content, char* cur_pid) {
    int count = 0;
    //usleep(10000);
    char* ppid = get_ppid_from_stat(content);
    if (strcmp(ppid, cur_pid) == 0) {
        count++;
        // count for pid
        if (strlen(ppid) > 1) {
            char* pid = get_pid_from_stat(content);
            //printf("search for ppid= %s (count %d)\n", pid, count);
            int child_count = proc_dir("stat", count_childrens, pid);
            //printf("child count %d\n", child_count);
            count += child_count;
        } else {
            //printf("end 1\n");
        }
    }
    return count;
}

int compare_and_count_name(char* content, char* _) {
    static char name[] = "tmux";
    static int count = 0;
    if (strcmp(content, name) == 0) {
        count++;
    }
    return count;
}

int proc_dir(char* fn, int(*fun)(char*, char*), char* cur_pid) {
    //static int level = 0;
    //level++;
    //printf("level %d\n", level);
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

void my_sig_int(int x) {
    printf("(%d) Ты закрыл меня!\n", x);
    exit(0);
}

int main(int argc, char** argv) {
    int count;
    //count = proc_dir("comm", compare_and_count_name, NULL);
    //printf("%d\n", count);
    signal(SIGINT, my_sig_int);
    //while(1);

    if (argc < 2) return 0;
    count = 0;
    count = proc_dir("stat", count_childrens, argv[1]);
    printf("%d\n", count);
    return 0;
}
