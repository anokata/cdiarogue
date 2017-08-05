//  socket, bind(ip? port), sendto / recvfrom
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char** argv) {
    if (argc < 2) return 0;
    struct hostent *h;
    h = gethostbyname(argv[1]);
    if (h == NULL) {
        perror("host");
        return 0;
    }
    printf("hostname: %s\n", h->h_name);
    int i = 0;
    while (h->h_aliases[i] != NULL) printf("alias: %s\n", h->h_aliases[i++]);
    printf("type: %s\n", (h->h_addrtype == AF_INET) ? "ipv4":"ipv6");
    printf("len: %d\n", h->h_length);
    //struct in_addr* adr = (struct in_addr*) h
    printf("addr: %s\n", inet_ntoa(*(struct in_addr*)h->h_addr));
    i = 0;
    while (h->h_addr_list[i] != NULL)
        printf("addr: %s\n", inet_ntoa(*(struct in_addr*)h->h_addr_list[i++]));
    // inet_ntoa();
    return 0;
}
