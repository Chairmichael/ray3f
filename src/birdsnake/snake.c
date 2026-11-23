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

void free_snakegame(SnakeGame game)
{
    free_ring_buffer(&game.snake);
}

SnakeGame init_snakegame(size_t tiles_x, size_t tiles_y, size_t snake_size)
{
    SnakeGame game = {0};
    // set minimums and maximums
    snake_size = (size_t)Clamp(snake_size, MIN_SNAKE_SIZE, MAX_SNAKE_SIZE);
    tiles_x = (size_t)Clamp(tiles_x, MIN_BOARD_SIZE, MAX_BOARD_SIZE);
    tiles_y = (size_t)Clamp(tiles_y, MIN_BOARD_SIZE, MAX_BOARD_SIZE);

    game.state = GS_PAUSED;
    game.bounds.x = tiles_x - 1;
    game.bounds.y = tiles_y - 1;
    game.snake = create_ring_buffer(tiles_x * tiles_y);

    // check if snake size is too large
    if (snake_size > tiles_x - 1 && snake_size > tiles_y - 1)
    {
        printf("Snake size reduced from %d", (int)snake_size);
        snake_size = tiles_x > tiles_y ? tiles_x - 1 : tiles_y - 1; // cap snake size
        printf(" to %d\n", (int)snake_size);
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
        rb_prepend(&game.snake, random_vector2(0, snake_size - 1, tiles_x - 1, tiles_y - 2));
    else if (game.snake_dir == DIR_UP)
        rb_prepend(&game.snake, random_vector2(0, 1, tiles_x - 1, tiles_y - snake_size));
    else if (game.snake_dir == DIR_LEFT)
        rb_prepend(&game.snake, random_vector2(1, 0, tiles_x - snake_size, tiles_y - 1));
    else if (game.snake_dir == DIR_RIGHT)
        rb_prepend(&game.snake, random_vector2(snake_size - 1, 0, tiles_x - 2, tiles_y - 1));

    // create rest of the body backwards from the head
    for (size_t i = 0; i < snake_size - 1; i++)
    {
        Vector2 new_segment = Vector2Subtract(rb_peek_tail(game.snake), dir_to_vector2(game.snake_dir));
        rb_append(&game.snake, new_segment);
    }

    // init apple
    game.apple = get_apple(game.snake, game.bounds);

    return game;
}

void snakegame_step(SnakeGame *game)
{
    if (game->state != GS_NORMAL)
        game->state = GS_NORMAL;

    game->snake_dir = game->snake_pending;
    move_snake(&game->snake, game->snake_dir);

    // check if apple was eaten
    if (Vector2Equals(rb_peek_head(game->snake), game->apple))
    {
        // only save because snake was just popped from tail
        game->snake.count++;
        // check if won, otherwise get new apple
        if (rb_is_full(game->snake))
            game->state = GS_WON;
        else
            game->apple = get_apple(game->snake, game->bounds);
    }
    else if (snake_eaten_self(game->snake) || snake_outbounds(game->snake, game->bounds))
    {
        game->state = GS_LOST;
    }
}

void move_snake(RingBuffer *snake, Direction dir)
{
    Vector2 out, head = rb_peek_head(*snake);
    rb_prepend(snake, Vector2Add(head, dir_to_vector2(dir)));
    rb_pop_tail(snake, &out);
}

Vector2 get_apple(RingBuffer snake, Vector2 bounds)
{
    Vector2 apple;
    while (true)
    {
        apple = random_vector2(0, 0, bounds.x, bounds.y);
        if (!rb_contains(snake, apple))
            return apple;
    }
}

bool snake_outbounds(RingBuffer snake, Vector2 bounds)
{
    Vector2 head = rb_peek_head(snake);
    return head.x < 0 || head.y < 0 || head.x > bounds.x || head.y > bounds.y;
};

bool snake_eaten_self(RingBuffer snake)
{
    if (snake.count <= 3)
        return false;
    Vector2 head = rb_peek_head(snake);
    for (size_t i = 1; i < snake.count; i++)
    {
        if (Vector2Equals(rb_peek_element(&snake, i), head))
            return true;
    }
    return false;
}