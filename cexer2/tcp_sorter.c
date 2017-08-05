#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define BUFSIZE 5*1024
struct sockaddr_in local;

int comparator(const void* a, const void* b) {
    if ((*(char *)a) < (*(char*)b)) {
        return 1;
    } else {
        return -1;
    }
}

int server(unsigned int port) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    int cs;
    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(port);
    local.sin_family = AF_INET;
    int result = bind(s, (struct sockaddr*) &local, sizeof(local));
    if (result) {
        perror("bind");
        return 0;
    }
    listen(s, 5);
    cs = accept(s, NULL, NULL);

    char buf[BUFSIZE];
    int bytes = read(cs, buf, BUFSIZE); 
    while (strncmp(buf, "OFF", 3) && bytes) {
        qsort(buf, bytes, sizeof(char), &comparator);
        write(cs, buf, bytes);
        //printf("%s\n", buf);
        bytes = read(cs, buf, BUFSIZE); 
    }
    close(cs);
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return 0;
    }
    server(atoi(argv[1]));
    return 0;
}
