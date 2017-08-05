#include <stdio.h>
#include <stddef.h>
#include <dlfcn.h>

void (*hello_message)(const char *);
int (*runtime_fun)(int);

int main(int argc, char *argv[]) {
    char *s = "Str";
    void *lib = dlopen("./libHello.so", RTLD_LAZY);
    if (NULL == lib)
        printf("lib not loaded");
    hello_message = (void (*) (const char *))dlsym(lib, "hello_message");
    runtime_fun = (int (*) (int))dlsym(lib, "runtime_fun");
    if (NULL == runtime_fun || NULL == hello_message)
        printf("funcs not founded");
    hello_message("11YEar!");
    hello_message(s);
    runtime_fun(123);
    return 0;
}
//gcc main.c -L. -lHello -o hello
//run as: LD_LIBRARY_PATH=. ./hello
