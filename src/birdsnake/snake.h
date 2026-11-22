#ifndef SNAKE_H
#define SNAKE_H

#include "raylib.h"
#include "util.h"
#include <cstdlib>

typedef enum
{
    DIR_NONE = -1,
    DIR_UP,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
} Direction;

typedef struct
{
    Rectangle board;
    GameState state;
    struct
    {
        Vector2 *body;
        size_t head, tail;
        Direction dir, pending;
    } snake;
} SnakeGame;

#endif