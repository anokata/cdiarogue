#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct Anim {
    uint8_t frame;
} *Anim;

Anim anim_make();
void anim_free(Anim a);
bool anim_step();
