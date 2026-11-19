#include "raylib.h"
#include "game.h"
#include "snake_assets.h"

int main(void)
{
    InitWindow(1800, 1200, "4k snake");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Rectangle board_shape;
    Snake snake;
    Vec2i apple;
    bool paused = true;

#ifdef RAY3F_snake_ASSETS_H
    Image apple_img = LoadImageFromMemory(".png", apple_png, apple_png_len);
    Texture2D apple_texture = LoadTextureFromImage(apple_img);
#else
    Texture2D apple_texture = LoadTexture("assets/snake/apple.png");
#endif

    game_init(&board_shape, &snake, &apple);

    double delta = GetTime();
    GameStatus status = GS_NORMAL;
    while (!WindowShouldClose())
    {
        game_step(&status, &board_shape, &snake, &apple, &delta, &paused, &apple_texture);
    }
    if (IsWindowReady())
        CloseWindow();
    return 0;
}