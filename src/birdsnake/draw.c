#include "draw.h"

#define BIRDSNAKE_ASSETS_IMPLEMENTATION
#include "birdsnake_assets.h"

void load_textures(Texture2D textures[TEXTURE_COUNT])
{
    Image apple_img = LoadImageFromMemory(".png", apple_png, apple_png_len);
    Texture2D apple_texture = LoadTextureFromImage(apple_img);
    UnloadImage(apple_img);
    textures[TX_APPLE] = apple_texture;
}

void draw_texture_scaled_t(Texture tex, Rectangle rec, Color tint)
{
    Rectangle tex_rec = {0, 0, tex.width, tex.height};
    DrawTexturePro(tex, tex_rec, rec, (Vector2){0}, 0, tint);
}

void draw_texture_scaled(Texture tex, Rectangle rec)
{
    draw_texture_scaled_t(tex, rec, WHITE);
}
