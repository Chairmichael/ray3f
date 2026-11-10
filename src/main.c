#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raygui.h"

int main(void)
{
    InitWindow(800, 450, "My Game");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello raylib!", 100, 200, 20, DARKGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}