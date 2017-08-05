#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    const int BUFSIZE = 30;
    char *proc_path = "/proc/self/status";
    char *ppid_str = "PPid";
    FILE *f = fopen(proc_path, "r");
    if (!f) {
        perror("Open: ");
        return 0;
    }
    char *pos = 0;
    char str[BUFSIZE+1];
    char *res = str;

    while (!pos && res) {
        res = fgets(str, BUFSIZE, f);
        pos = strstr(str, ppid_str);
        //printf("%s, %s", str, pos);
    }
    //printf("%s%s", str, pos);
    if (pos == 0) return 0;
    char ppid_s[BUFSIZE];
    int k = 0;
    int len = strlen(str);
    int i = 0;
    for (i = 0; i < len; i++) {
        if (isdigit(str[i])) {
            ppid_s[k++] = str[i];
        }
    }
    ppid_s[k++] = '\0';
    //printf("%s", ppid_s);
    int ppid = atoi(ppid_s);
    printf("%d\n", ppid);

    if (fclose(f)) {
        perror("Close: ");
        return 0;
    }
    return 0;
}
