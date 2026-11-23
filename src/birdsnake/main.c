#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "raylib.h"

#include "draw.h"
#include "snake.h"

#define WIDTH_SCALE 0.65f
#define WINDOWED_ASPECT_RATIO (3.0 / 2.0)
#define SMALLEST(x, y) (x < y ? x : y)
#define LARGEST(x, y) (x > y ? x : y)

static Vector2 setup_window(bool fullscreen)
{
    if (!IsWindowReady())
    {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(600, 400, "BIRDSNAKE!!!");
        SetWindowMinSize(600, 400);
    }

    int monitor = GetCurrentMonitor();
    int width = GetMonitorWidth(monitor) * WIDTH_SCALE;
    Vector2 size = {(float)width, (float)(width / WINDOWED_ASPECT_RATIO)};

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

static Vector2 update_window(bool fullscreen)
{
    if (!IsWindowFullscreen() && fullscreen)
        ToggleBorderlessWindowed();
    else if (IsWindowFullscreen() && !fullscreen)
        ToggleBorderlessWindowed();

    return (Vector2){(float)GetScreenWidth(), (float)GetScreenHeight()};
}

int main(int argc, char *argv[])
{
    bool fullscreen = false;
    Vector2 window_size = setup_window(fullscreen);
    SetRandomSeed(time(NULL));

    Texture2D textures[TEXTURE_COUNT];
    load_textures(textures);

    SnakeGame snakeG;
    if (argc == 2)
        snakeG = init_snakegame(atoi(argv[1]), atoi(argv[1]), DEFUALT_SNAKE_SIZE);
    else if (argc == 4)
        snakeG = init_snakegame(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    else
        snakeG = init_snakegame(DEFUALT_BOARD_SIZE_X, DEFUALT_BOARD_SIZE_Y, DEFUALT_SNAKE_SIZE);

    while (!WindowShouldClose())
    {
        if (IsWindowResized())
            window_size = update_window(false);

        BeginDrawing();
        ClearBackground(GRAY);
        EndDrawing();

        if (IsKeyPressed(KEY_ENTER))
            fullscreen = !fullscreen;
    }

    if (IsWindowReady())
        CloseWindow();
    return 0;
}