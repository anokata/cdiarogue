#include <stdio.h>
#include <stdlib.h>

int count_zero(char* cmd) {
    int count = 0;
    //puts(cmd);

    FILE* fd = popen(cmd, "r");
    if (fd == 0) {
        perror("popen");
        exit(2);
    }

    char c = 1;
    while (c != EOF) {
        //fread(&c, 1, 1, fd);
        c = fgetc(fd);
        //putc(c, stdout);
        if (c == '0') count++;
    }

    int s = pclose(fd);
    //printf("pclose - %d\n", s);
    if (s != 0) {
        perror("pclose");
        exit(2);
    }
    return count;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("0\n");
    } else {
        char cmd[128];
        sprintf(cmd, "./%s %s", argv[1], argv[2]);
        printf("%d\n", count_zero(cmd));
    }
    return 0;
}
