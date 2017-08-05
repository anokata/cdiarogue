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
    int i = 0;
    i = 0;
    while (h->h_addr_list[i] != NULL)
        printf("%s\n", inet_ntoa(*(struct in_addr*)h->h_addr_list[i++]));
    return 0;
}
