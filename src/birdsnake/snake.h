#ifndef SNAKE_H
#define SNAKE_H

#include "raylib.h"
#include "raymath.h"
#include "util.h"
#include <stddef.h>

#define MIN_SNAKE_SIZE 3
#define MIN_BOARD_SIZE (MIN_SNAKE_SIZE + 1)

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
    RingBuffer snake;
    Direction snake_dir, snake_pending;
    Vector2 apple;
} SnakeGame;

SnakeGame init_snake_game(size_t tiles_x, size_t tiles_y, size_t snake_size);
void free_snake_game(SnakeGame game);

#endif