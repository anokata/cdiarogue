#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <dlfcn.h>

int (*func)(int);

int main(int argc, char *argv[]) {
    // printf("Args: lib '%s' fun '%s' param '%s'\n", argv[1], argv[2], argv[3]);
    // const char *dir = "./";
    void *lib = dlopen(argv[1], RTLD_LAZY);
    func = (int (*) (int)) dlsym(lib, argv[2]);
    int res = func(atoi(argv[3]));
    printf("%i\n", res);
    return 0;
}
