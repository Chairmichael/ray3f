#include "snake.h"

static inline Vector2 random_vector2(int x1, int y1, int x2, int y2)
{
    return (Vector2){GetRandomValue(x1, x2), GetRandomValue(y1, y2)};
}
static Vector2 dir_to_vector2(Direction dir)
{
    switch (dir)
    {
    case DIR_UP:
        return (Vector2){0, -1};
    case DIR_RIGHT:
        return (Vector2){1, 0};
    case DIR_DOWN:
        return (Vector2){0, 1};
    case DIR_LEFT:
        return (Vector2){-1, 0};
    default:
        return (Vector2){0, 0};
    }
}

void free_snake_game(SnakeGame game)
{
    free_ring_buffer(&game.snake);
}

SnakeGame init_snake_game(size_t tiles_x, size_t tiles_y, size_t snake_size)
{
    SnakeGame game = {0};
    // minimums
    snake_size = snake_size < MIN_SNAKE_SIZE ? snake_size = MIN_SNAKE_SIZE : snake_size;
    tiles_x = tiles_x < MIN_BOARD_SIZE ? tiles_x = MIN_BOARD_SIZE : tiles_x;
    tiles_y = tiles_y < MIN_BOARD_SIZE ? tiles_y = MIN_BOARD_SIZE : tiles_y;

    game.board.x = tiles_x - 1;
    game.board.y = tiles_y - 1;
    game.snake = create_ring_buffer(tiles_x * tiles_y);

    // check if snake size is too large
    if (snake_size > tiles_x - 1 && snake_size > tiles_y - 1)
    {
        printf("Snake size reduced from %d", snake_size);
        snake_size = tiles_x > tiles_y ? tiles_x - 1 : tiles_y - 1; // cap snake size
        printf(" to %d\n", snake_size);
        game.snake_dir = GetRandomValue(0, 3);
    }
    else if (snake_size > tiles_x - 1)
        game.snake_dir = GetRandomValue(0, 1) ? DIR_DOWN : DIR_UP;
    else if (snake_size > tiles_y - 1)
        game.snake_dir = GetRandomValue(0, 1) ? DIR_LEFT : DIR_RIGHT;
    else
        game.snake_dir = GetRandomValue(0, 3);
    game.snake_pending = game.snake_dir;

    // make sure snake head spawns at least 1 tile away from the edge its facing
    if (game.snake_dir == DIR_DOWN)
        prepend_rb(&game.snake, random_vector2(0, snake_size - 1, tiles_x - 1, tiles_y - 2));
    else if (game.snake_dir == DIR_UP)
        prepend_rb(&game.snake, random_vector2(0, 1, tiles_x - 1, tiles_y - snake_size));
    else if (game.snake_dir == DIR_LEFT)
        prepend_rb(&game.snake, random_vector2(1, 0, tiles_x - snake_size, tiles_y - 1));
    else if (game.snake_dir == DIR_RIGHT)
        prepend_rb(&game.snake, random_vector2(snake_size - 1, 0, tiles_x - 2, tiles_y - 1));

    // create rest of the body backwards from the head
    for (size_t i = 0; i < snake_size - 1; i++)
    {
        Vector2 new_segment = Vector2Subtract(peek_tail_rb(game.snake), dir_to_vector2(game.snake_dir));
        append_rb(&game.snake, new_segment);
    }

    // init apple
    while (true)
    {
        game.apple = random_vector2(0, 0, tiles_x - 1, tiles_y - 1);
        if (!contains_rb(game.snake, game.apple))
            break;
    }
}

