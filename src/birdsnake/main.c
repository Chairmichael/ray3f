#include "birdsnake_assets.h"
#include "draw.h"
#include "raylib.h"
#include <stdio.h>

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
    SetRandomSeed(time());

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
        if (IsKeyPressed(KEY_ENTER))
            ToggleBorderlessWindowed();
        if (IsWindowResized())
        {
            x++;
            snprintf(text, sizeof text, "%d", x);
            window_size = update_window(false);
            window_rect.width = window_size.x;
            window_rect.height = window_size.y;
            rect = (Rectangle){
                window_size.x / 2 - window_size.x / 5 / 2,
                window_size.y / 2 - window_size.y / 5 / 2,
                window_size.x / 5,
                window_size.x / 5,
            };
        }
        BeginDrawing();
        ClearBackground(GRAY);
        draw_texture_scaled(apple_texture, window_rect);

        DrawText("This is game text", 80, 50, 60, ORANGE);
        DrawRectangleRec(rect, ORANGE);
        DrawText(text, 10, 10, 40, ORANGE);
        if (IsKeyDown(KEY_SPACE))
        {
            // gray background
            DrawRectangleRec(window_rect, (Color){0, 0, 0, 150});
            float menu_scale = 0.7;
            int menu_size = SMALLEST(window_size.x, window_size.y) * menu_scale;
            Vector2 menu_margin = {
                (window_size.x - menu_size) / 2.0f,
                (window_size.y - menu_size) / 2.0f,
            };
            Rectangle menu_rec = {
                menu_margin.x,
                menu_margin.y,
                menu_size,
                menu_size,
            };
            DrawRectangleRec(menu_rec, BLUE);
        }
        EndDrawing();
    }

    if (IsWindowReady())
        CloseWindow();
    return 0;
}