#include "draw.h"

void draw_texture_scaled_t(Texture tex, Rectangle rec, Color tint)
{
    Rectangle tex_rec = {0, 0, tex.width, tex.height};
    DrawTexturePro(tex, tex_rec, rec, (Vector2){0}, 0, tint);
}

void draw_texture_scaled(Texture tex, Rectangle rec)
{
    draw_texture_scaled_t(tex, rec, WHITE);
}
