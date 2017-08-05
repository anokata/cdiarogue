#include <stdio.h>

void print_array(int arr[], int len) {
    int i;
    for (i = 0; i < len; i++) {
        printf(" %d", arr[i]);
    }
}

void linear_sort(int arr[], int len) {
    int i, j, t;
    for (i = 0; i < len; i++) {
        for (j = i; j > 0 && (arr[j - 1] > arr[j]); j--) {
            //printf("\n%d <=> %d\n", arr[j-1], arr[j]);
            //print_array(arr, len);
            t = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = t;
        }
    }
}

#define LEN 20
int main() {
    int arr[LEN] = {6,5,4,3,1,2,3,4,5,6,9,7,8,7,8,0,2};
    linear_sort(arr, LEN);
    size_t i;
    for (i = 0; i < LEN; i++) {
        printf(" %d", arr[i]);
    }

    return 0;
}
