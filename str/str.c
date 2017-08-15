#include <stdio.h>

#define xprint(x, f) printf(#x " -> %"f"\n", x);

int main() {
    printf("str functions tests\n");
    printf("* strspn\n");
    xprint(strspn("abcde", "cba"), "d");
    xprint(strpbrk("abcde", "cd"), "s");
    xprint(strpbrk("_____123___", "0123"), "s");
    xprint(strpbrk("________", "0123"), "s");
}
