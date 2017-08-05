#include <stddef.h>

int stringStat(const char *string, size_t multiplier, int *count) {
    (*count)++;
    int l = 0;
    for (l = 0; string[l] != 0; ++l);
    return l * multiplier;
}
