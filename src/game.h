#ifndef GAME_H
#define GAME_H

#include "stdlib.h"
#include "raylib.h"
#include "raymath.h"

#define SCR_WORLD_PADDING 0.05
#define BOARD_LEN 5
#define BOARD_LIMIT (BOARD_LEN - 1)
#define SNAKE_MAX (BOARD_LEN * BOARD_LEN)
#define START_SNAKE_LEN 3
#define SPAWN_BUFFER 2
#define GAME_SPEED 0.25
#define SCORE_STRING_LEN 10
#define INDEX(snake, i) (((snake)->tail + (i)) % SNAKE_MAX)
#define CLAMP(value, min, max) (value < min ? min : value > max ? max \
                                                                : value)

#define SCR_FONT_FRACTION 10
#define DARKERGREEN (Color){0, 40, 5, 255}
#define ROUNDNESS 0.2
#define ROUNDED_SEGMENTS 8

typedef enum
{
    GS_NORMAL,
    GS_LOST,
    GS_WIN,
} GameStatus;

typedef enum
{
    DIR_NONE = -1,
    DIR_UP,
    DIR_LEFT,
    DIR_DOWN,
    DIR_RIGHT,
} Direction;

typedef struct
{
    int x, y;
} Vec2i;

typedef struct
{
    Vec2i body[SNAKE_MAX];
    int head, tail;
    Direction dir, pending;
} Snake;

void game_init(Rectangle *board_shape, Snake *snake, Vec2i *apple);
void game_step(Rectangle *board_shape, Snake *snake, Vec2i *apple, double *detla, bool *paused, Texture2D *apple_texture);
void game_draw(Rectangle *board_shape, Snake *snake, Vec2i *apple, Texture2D *apple_texture);
void get_board_size(Rectangle *board_shape);
Direction handle_input(Rectangle *board_shape, bool *restart, bool *paused);
void move_snake(Snake *snake);
void check_apple(Snake *snake, Vec2i *apple);
Vec2i get_random_point_excluding(Snake *snake, int max_x, int max_y);
GameStatus check_snake(Snake *snake);
void set_snake_dir(Direction cur_dir, Direction *pending_dir, Direction input_dir);
void print_snake(Snake *snake);
int len(Snake *snake);
Vec2i *next(Snake *snake);

#endif