#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    int fd1, fd2;

    mkfifo("in1", 0666);
    mkfifo("in2", 0666);
    fd1 = open("in1", O_WRONLY );
    fd2 = open("in2", O_WRONLY );
    printf("%d %d\n", fd1, fd2);
    perror("open");

    getchar();
    printf("get1\n");
    write(fd1, "10", 2);
    write(fd2, "20", 2);
    getchar();
    printf("get2\n");
    write(fd2, "4", 1);
    write(fd1, "1", 1);

    close(fd1);
    perror("close");
    close(fd2);
    perror("close");
    unlink("in1");
    perror("unlink");
    unlink("in2");
    perror("unlink");
    printf("end\n");

    return 0;
}
