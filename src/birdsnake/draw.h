#ifndef DRAW_H
#define DRAW_H

#include "raylib.h"

typedef enum
{
    TX_APPLE,
    TEXTURE_COUNT,
} TextureDictionary;

void load_textures(Texture2D textures[TEXTURE_COUNT]);
void draw_texture_scaled(Texture tex, Rectangle rec);
void draw_texture_scaled_t(Texture tex, Rectangle rec, Color tint);

#endif