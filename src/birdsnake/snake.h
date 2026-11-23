#ifndef SNAKE_H
#define SNAKE_H

#include "raylib.h"
#include "raymath.h"
#include "util.h"
#include <stddef.h>
#include <stdio.h>

#define DEFUALT_BOARD_SIZE_X 5
#define DEFUALT_BOARD_SIZE_Y 6
#define MIN_SNAKE_SIZE 3
#define DEFUALT_SNAKE_SIZE MIN_SNAKE_SIZE
#define MIN_BOARD_SIZE (MIN_SNAKE_SIZE + 1)
#define MAX_BOARD_SIZE 1000
#define MAX_SNAKE_SIZE (MAX_BOARD_SIZE - 1)

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
    Rectangle display;
    Vector2 bounds;
    GameState state;
    RingBuffer snake;
    Direction snake_dir, snake_pending;
    Vector2 apple;
} SnakeGame;

SnakeGame init_snakegame(size_t tiles_x, size_t tiles_y, size_t snake_size);
void free_snakegame(SnakeGame game);
void snakegame_step(SnakeGame *game);
void move_snake(RingBuffer *snake, Direction dir);
Vector2 get_apple(RingBuffer snake, Vector2 bounds);
bool snake_outbounds(RingBuffer snake, Vector2 bounds);
bool snake_eaten_self(RingBuffer snake);

#endif