#include "anim.h"
#include <stdio.h>
#include <assert.h>

void test_create() {
    Anim a = anim_make();
    a->frame = 3;
    anim_step(a);
    anim_free(a);
}

int main() {
    test_create();
}
