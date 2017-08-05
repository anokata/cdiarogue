//#include <glib.h>
#include <stdio.h>

int main() {
    int count = 0;
    int d = 1024*1024;
    for (int i = 0; i < 65536; i++) {
        for (int j = 0; j < 65536; j++) {
            if (((i * 16 + j) % d) == 0x7c00) {
                count++;
            }
        }
    }
    printf("%d", count);
    return 0;
}
