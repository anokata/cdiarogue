#include <stdio.h>
#include <string.h>

int main () {
    int size = 8;
    int arr[size];
    arr[size-1] = size;
    arr[1] = size;
    arr[0] = size;
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    double d = 2.2;
    printf("%f\n", d/0.0);
    printf("%f\n", 0.0/0.0);
    printf("%d\n", 0/0);
    // alloca
    //
    return 0;
}

