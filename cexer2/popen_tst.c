#include <stdio.h>

int main() {
    FILE* fd = popen("touch some", "r");
    if (fd == 0) perror("popen");

    pclose(fd);
    return 0;
}
