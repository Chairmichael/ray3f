#include <stdio.h>
#include "raylib.h"
// #include "birdsnake_assets.h"

#define WIDTH_SCALE 0.65f
#define WINDOWED_ASPECT_RATIO (3.0 / 2.0)

typedef struct
{
    int x, y;
} Vec2;

static Vec2 update_window(bool fullscreen)
{
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
    if (fullscreen)
        ToggleBorderlessWindowed();

    SetTargetFPS(GetMonitorRefreshRate(monitor));
    return size;
}

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(500, 500, "BIRDSNAKE!!!");
    Vec2 window_size = update_window(false);

    Rectangle board_shape;

    // #ifdef RAY3F_birdsnake_ASSETS_H
    //     Image apple_img = LoadImageFromMemory(".png", apple_png, apple_png_len);
    //     Texture2D apple_texture = LoadTextureFromImage(apple_img);
    // #else
    //     Texture2D apple_texture = LoadTexture("assets/birdsnake/apple.png");
    // #endif
    int x = 0;
    char text[12] = "0";
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ENTER))
            ToggleBorderlessWindowed();
        BeginDrawing();
        ClearBackground(GRAY);
        if (IsWindowResized())
        {
            x++;
            snprintf(text, sizeof text, "%d", x);
        }

        DrawText(text, 10, 10, 40, ORANGE);
        EndDrawing();
    }
    if (IsWindowReady())
        CloseWindow();
    return 0;
}