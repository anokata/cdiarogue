#include <stdio.h>

void reverse(char *str, int len);

extern void some(void);

int main() {
    char *buf = NULL;
    size_t size = 0;
    int len = getline(&buf, &size, stdin);
    printf("%d\n%s", len, buf);
    reverse(buf, len);
    printf("%s", buf);
    /* some(); */
}

void reverse(char *str, int len) {
    /* printf("rev in:%d\n%s", len, str); */
for (int i = 0; i < len / 2; ++i) { // SEE DIV TWO: must run to middle of string, else swap back to normal
        /* printf("step %d (%c)\n", i, str[i]); */
        char t = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = t;
    }
}
