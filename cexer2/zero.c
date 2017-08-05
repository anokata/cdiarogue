#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("0123012302");
    } else {
        int count = atoi(argv[1]);
        while (count--) printf("0%d0_0", count*count);
    }
    return 0;
}
