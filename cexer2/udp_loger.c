#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define BUFSIZE 5*1024

struct sockaddr_in local;

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Use [progname] port\n");
        return 0;
    }
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == -1) {
        perror("socket");
        return 0;
    }
    
    unsigned int port = atoi(argv[1]);
    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(port);
    local.sin_family = AF_INET;

    int result = bind(s, (struct sockaddr*) &local, sizeof(local));
    if (result == -1) {
        perror("bind");
        return 0;
    }
    
    char buf[BUFSIZE];
    int bytes = read(s, buf, BUFSIZE);
    buf[bytes] = '\0';
    while(strcmp(buf, "OFF\n")) {
        printf("%s\n", buf);
        bytes = read(s, buf, BUFSIZE);
        buf[bytes] = '\0';
    }

    return 0;
}
