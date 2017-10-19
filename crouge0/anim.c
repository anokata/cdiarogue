#include "anim.h"

Anim anim_make() {
    Anim a = malloc(sizeof(struct Anim));
    memset(a, 0, sizeof(struct Anim));
    return a;
}

void anim_free(Anim a) {
    free(a);
}

bool anim_step(Anim anim) {

    return --(anim->frame) == 0;
}
