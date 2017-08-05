#include <stdio.h>
#define INWORD 1
#define OUTWORD 0

int main() { 
    printf("Words: \n");
    int c_count = 0;
    int w_count = 0;
    int l_count = 0;
    int word_len = 0;
    int state = INWORD;
    int c = getchar();
    int word_lens[10] = {0};
    while (c != EOF) {
        if (c == '\n') ++l_count;
        ++c_count;
        if (state == INWORD) {
            putchar(c);
            ++word_len;
            if (c == ' ' || c == '\t' || c == '\n') {
                state = OUTWORD;
                ++w_count;
                ++word_lens[--word_len];
                word_len = 0;
            }
        } else {
            putchar('\n');
            if (c != ' ' && c != '\n' && c != '\t') {
                ++word_len;
                putchar(c);
                state = INWORD;
            }
        }
        c =getchar();
    }

    printf("\n");
    printf("bytes(%d) ", c_count);
    printf("lines(%d) ", l_count);
    printf("words(%d) ", w_count);
    /* Hystogramm */
    for (int i = 0; i < 10; i++) {
        printf("%d: %d ", i, word_lens[i]);
        for (int j = 0; j < word_lens[i]; j++) {
            putchar('*');
        }
        putchar('\n');
    }

    printf("\n");
}
