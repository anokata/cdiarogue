#include "hello.h"
#include <stdio.h>

void hello_message(const char *name) {
    printf("Hello %s!\n", name);
}
// compile to Shared lib: gcc -o libName.so -shared -fPIC file.c
// nm - !!! view names. nm libSome.so
int runtime_fun(int a) {
    printf("from dinamyc loaded\n");
    return a*a*a;
}
