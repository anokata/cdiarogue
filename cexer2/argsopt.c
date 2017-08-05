#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

int main(int argc, char** argv) {
    char str[] = "some string";
    int ib = 0;
    int i;
    int c;
    int len=0;

    struct option opts[] = {
        {"br", no_argument, &ib, 1},
        {"len", required_argument, 0, 'l'},
        {"an", no_argument, 0, 'a'},

        {"query", required_argument, 0, 'q'},
        {"longinformationrequest", no_argument, 0, 'i'},
        {"version", no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };
    int correct = 1;
    while((c = getopt_long(argc, argv, "q:iv", opts, &i)) != -1) {
        switch(c) {
            case 0:
                printf("0\n");
                break;
            case 'l':
                len = atoi(optarg);
                printf("length=%d\n", len);
                break;
            case 'a':
                printf("(c) noname\n");
                break;
            case 'q':
                if (strlen(optarg) < 1) 
                    correct = 0;
                break;
            case 'i':
                break;
            case 'v':
                break;
            default:
                correct = 0;
                printf("optchar: %c\n", c);
        }
    }

    /*
    if (strlen(str) > len && len != 0)
        str[len] = '\0';
    if (ib)
        printf("[%s]\n", str);
    else 
        printf("%s\n", str);
        */
    if (correct)
        printf("+\n");
    else 
        printf("-\n");

    return 0;
}
