#include "object.h"
#include <stdio.h>
#include <assert.h>

int main() {
    Object o = object_new(1, 2, 'z');
    object_print(o);
    object_free(o);
}
