#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef void (*AnimFunc)(void *);

typedef struct Anim {
    uint8_t frame;
    AnimFunc fun;
    char c;
    int sx;
} *Anim;

Anim anim_make();
void anim_free(Anim a);
bool anim_step();
