#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "game.h"

int main(void)
{
    InitWindow(1800, 1200, "4k snake");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Rectangle board_shape;
    Snake snake;
    Vec2i apple;
    bool paused = true;

    Texture2D apple_texture = LoadTexture("assets/apple.png");

    game_init(&board_shape, &snake, &apple);

    double delta = GetTime();
    while (!WindowShouldClose())
    {
        game_step(&board_shape, &snake, &apple, &delta, &paused, &apple_texture);
    }
    if (IsWindowReady())
        CloseWindow();
    return 0;
}