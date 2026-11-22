#ifndef DRAW_H
#define DRAW_H

#include "raylib.h"

void draw_texture_scaled(Texture tex, Rectangle rec);
void draw_texture_scaled_t(Texture tex, Rectangle rec, Color tint);

#endif