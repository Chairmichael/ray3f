#include "birdsnake_assets.h"
#include "draw.h"
#include "raylib.h"
#include <stdio.h>
#include <time.h>

#define WIDTH_SCALE 0.65f
#define WINDOWED_ASPECT_RATIO (3.0 / 2.0)
#define SMALLEST(x, y) (x < y ? x : y)
#define LARGEST(x, y) (x > y ? x : y)

typedef struct
{
    int x, y;
} Vec2;

static Vec2 setup_window(bool fullscreen)
{
    if (!IsWindowReady())
    {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(600, 400, "BIRDSNAKE!!!");
        SetWindowMinSize(600, 400);
    }

    int monitor = GetCurrentMonitor();
    int width = GetMonitorWidth(monitor) * WIDTH_SCALE;
    Vec2 size = {width, width / WINDOWED_ASPECT_RATIO};

    if (!IsWindowFullscreen() && !fullscreen)
    {
        SetWindowSize(size.x, size.y);
        Vector2 mpos = GetMonitorPosition(monitor);
        SetWindowPosition(mpos.x + (GetMonitorWidth(monitor) - size.x) / 2,
                          mpos.y + (GetMonitorHeight(monitor) - size.y) / 2);
    }
    else if (fullscreen)
        ToggleBorderlessWindowed();

    SetTargetFPS(GetMonitorRefreshRate(monitor));
    return size;
}

static Vec2 update_window(bool fullscreen)
{
    if (!IsWindowFullscreen() && fullscreen)
        ToggleBorderlessWindowed();
    else if (IsWindowFullscreen() && !fullscreen)
        ToggleBorderlessWindowed();

    return (Vec2){GetScreenWidth(), GetScreenHeight()};
}

int main(void)
{
    Vec2 window_size = setup_window(false);
    Rectangle window_rect = {0, 0, window_size.x, window_size.y};
    SetRandomSeed(time(NULL));

    Rectangle board_shape;

#ifdef RAY3F_birdsnake_ASSETS_H
    Image apple_img = LoadImageFromMemory(".png", apple_png, apple_png_len);
    Texture2D apple_texture = LoadTextureFromImage(apple_img);
#else
    Texture2D apple_texture = LoadTexture("assets/birdsnake/apple.png");
#endif
    int x = 0;
    char text[12] = "0";

    Rectangle rect = {
        window_size.x / 2 - window_size.x / 5 / 2,
        window_size.y / 2 - window_size.y / 5 / 2,
        window_size.x / 5,
        window_size.x / 5,
    };

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_A) && IsKeyPressed(KEY_L))
            printf("AL pressed\n");
        int key, sum = 0;
        while ((key = GetKeyPressed()))
            sum++;
        if (sum > 1)
            printf("%d\n", sum);
        BeginDrawing();
        ClearBackground(GRAY);
        EndDrawing();
    }

    if (IsWindowReady())
        CloseWindow();
    return 0;
}