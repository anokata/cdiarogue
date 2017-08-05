#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main() {
    int f1 = open("./in1", O_RDONLY );
    int f2 = open("./in2", O_RDONLY );
    if ((f1 <= 0) || (f2 <= 0)) {
        perror("open");
        return 0;
    }
    //printf("%d %d\n", f1, f2);

    int sum = 0;
    fd_set read_set;
    fd_set copyset;
    int ex = 1;
    char buf[100];
    FD_ZERO(&read_set);
    FD_SET(f1, &read_set);
    FD_SET(f2, &read_set);
    while(FD_ISSET(f1, &read_set) || FD_ISSET(f2, &read_set)) {
        copyset = read_set;
        ex = select(f2+1, &copyset, NULL, NULL, NULL);
        //printf("return fro mselect\n");
        if (ex < 0) {
            perror("select");
            return 1;
        }
        if (ex > 0) {
            int fd;
            if (FD_ISSET(f1, &copyset)) {
                fd = f1;
            }
            if (FD_ISSET(f2, &copyset)) {
                fd = f2;
            }
            int bytes = read(fd, buf, 99);
            if (bytes == 0) {
                //puts("close");
                close(fd);
                FD_CLR(fd, &read_set);
            } else {
                buf[bytes] = 0;
                sum += atoi(buf);
                //printf("%d\n", sum);
            }
        } else {
            //puts("time out\n");
        }
    }

    printf("%d\n", sum);
    //close(f1);
    //close(f2);
    return 0;
}
