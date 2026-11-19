#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "game.h"

static const char *direction_name(Direction direction)
{
    switch (direction)
    {
    case DIR_UP:
        return "up";
    case DIR_LEFT:
        return "left";
    case DIR_DOWN:
        return "down";
    case DIR_RIGHT:
        return "right";
    case DIR_NONE:
        return "none";
    default:
        return "unknown";
    }
}

static bool same(Vec2i v1, Vec2i v2)
{
    return v1.x == v2.x && v1.y == v2.y;
}

static Vec2i head(Snake *snake)
{
    return snake->body[snake->head];
}

static bool head_equals(Snake *snake, Vec2i vec)
{
    return head(snake).x == vec.x && head(snake).y == vec.y;
}

static bool contains(Snake *snake, Vec2i vec)
{
    for (int i = 0; i < len(snake); i++)
    {
        int index = INDEX(snake, i);
        if (same(snake->body[index], vec))
            return true;
    }
    return false;
}

static Vector2 rotate_about(Vector2 vec, Vector2 pivot, float angle)
{
    vec = Vector2Subtract(vec, pivot);
    vec = Vector2Rotate(vec, angle);
    vec = Vector2Add(vec, pivot);
    return vec;
}

static float dir_to_rad(Direction dir)
{
    float rads = dir >= DIR_RIGHT && dir <= DIR_LEFT ? dir * 90.0f : 0.0;
    return rads * DEG2RAD;
}

static Vec2i dir_to_vec2i(Direction dir)
{
    switch (dir)
    {
    case DIR_UP:
        return (Vec2i){0, -1};
    case DIR_RIGHT:
        return (Vec2i){1, 0};
    case DIR_DOWN:
        return (Vec2i){0, 1};
    case DIR_LEFT:
        return (Vec2i){-1, 0};
    default:
        return (Vec2i){0, 0};
    }
}
static Vec2i vec2i_add(Vec2i a, Vec2i b)
{
    return (Vec2i){a.x + b.x, a.y + b.y};
}
static Vec2i vec2i_subtract(Vec2i a, Vec2i b)
{
    return (Vec2i){a.x - b.x, a.y - b.y};
}
static Vec2i vec2i_rotate(Vec2i vec, int deg)
{
    int quarter_turns = ((deg / 90) % 4 + 4) % 4;

    switch (quarter_turns)
    {
    case 1:
        return (Vec2i){-vec.y, vec.x};
    case 2:
        return (Vec2i){-vec.x, -vec.y};
    case 3:
        return (Vec2i){vec.y, -vec.x};
    default:
        return vec;
    }
}

int len(Snake *snake)
{
    return (snake->head - snake->tail + SNAKE_MAX) % SNAKE_MAX + 1;
}

void game_init(Rectangle *board_shape, Snake *snake, Vec2i *apple)
{
    get_board_size(board_shape);
    SetRandomSeed((unsigned int)time(NULL));

    snake->dir = GetRandomValue(DIR_UP, DIR_RIGHT);
    snake->pending = snake->dir;
    int spawn_limit_left = 0, spawn_limit_top = 0;
    int spawn_limit_right = BOARD_LIMIT, spawn_limit_bottom = BOARD_LIMIT;

    // make sure snake spawns away from the edge its facing
    if (snake->dir == DIR_UP || snake->dir == DIR_DOWN)
    {
        spawn_limit_top = SPAWN_BUFFER;
        spawn_limit_bottom = BOARD_LIMIT - SPAWN_BUFFER;
    }
    else if (snake->dir == DIR_LEFT || snake->dir == DIR_RIGHT)
    {
        spawn_limit_left = SPAWN_BUFFER;
        spawn_limit_right = BOARD_LIMIT - SPAWN_BUFFER;
    }

    snake->head = START_SNAKE_LEN - 1;
    snake->tail = 0;
    snake->body[snake->head].x = GetRandomValue(spawn_limit_left, spawn_limit_right);
    snake->body[snake->head].y = GetRandomValue(spawn_limit_top, spawn_limit_bottom);
    Vec2i bounds = {BOARD_LIMIT, BOARD_LIMIT};

    for (size_t i = snake->head; i > snake->tail; i--)
    {
        Vec2i dir_vec = dir_to_vec2i(snake->dir);
        // make new segment by subtracting the direction vector from the snake body
        Vec2i new_segment = vec2i_subtract(snake->body[i], dir_vec);
        // if outside bounds, rotate the direction vector by 90 degrees
        if (!check_board_bounds(bounds, new_segment))
        {
            Vec2i shift = vec2i_rotate(dir_vec, 90);
            if (!check_board_bounds(bounds, vec2i_subtract(snake->body[i], shift)))
                shift = vec2i_rotate(dir_vec, -90);
            new_segment = vec2i_subtract(snake->body[i], shift);
        }
        snake->body[i - 1] = new_segment;
    }

    *apple = get_random_point_excluding(snake, BOARD_LIMIT, BOARD_LIMIT);
}

void get_board_size(Rectangle *game_size)
{
    int side_len = GetScreenWidth() > GetScreenHeight() ? GetScreenHeight() : GetScreenWidth();
    int padding = side_len * SCR_WORLD_PADDING;

    game_size->x = (GetScreenWidth() - side_len) / 2 + padding;
    game_size->y = (GetScreenHeight() - side_len) / 2 + padding;
    game_size->width = side_len - padding * 2;
    game_size->height = side_len - padding * 2;
    // printf("Board size: x=%d, y=%d, width=%d, height=%d\n",
    //        (int)game_size->x, (int)game_size->y, (int)game_size->width, (int)game_size->height);
}

void game_step(GameStatus *status, Rectangle *board_shape, Snake *snake, Vec2i *apple, double *last_step, bool *paused, Texture2D *apple_texture)
{
    bool restart = false;
    Direction input_dir = handle_input(board_shape, &restart, paused, *status);
    set_snake_dir(snake->dir, &snake->pending, input_dir);
    if (restart)
    {
        *status = GS_NORMAL;
        *paused = true;
        game_init(board_shape, snake, apple);
    }

    game_draw(*status, board_shape, snake, apple, apple_texture);

    if (!*paused && GetTime() - *last_step >= GAME_SPEED)
    {
        *last_step = GetTime();
        if (*status == GS_NORMAL)
            move_snake(snake);
        check_apple(snake, apple);
        if (apple->x == -1 && apple->y == -1)
            *apple = (Vec2i){-100, -100};
        *status = check_snake(snake);
    }
}

void game_draw(GameStatus status, Rectangle *board_shape, Snake *snake, Vec2i *apple, Texture2D *apple_texture)
{
    BeginDrawing();
    ClearBackground(BLANK);
    // drop shadow
    DrawRectangle(board_shape->x + board_shape->width * SCR_WORLD_PADDING,
                  board_shape->y + board_shape->width * SCR_WORLD_PADDING / 2,
                  board_shape->width,
                  board_shape->height,
                  (Color){76, 63, 47, 100});
    DrawRectangleRec(*board_shape, DARKBROWN);

    int tile_size = board_shape->width / BOARD_LEN;

    char score[SCORE_STRING_LEN];
    int font_size = GetScreenHeight() / SCR_FONT_FRACTION;
    snprintf(score, sizeof score, "%d", len(snake));
    int str_width = MeasureText(score, font_size);
    DrawText(score, board_shape->x - str_width - board_shape->y / 2, board_shape->y, font_size, ORANGE);

    float apple_scale = (float)tile_size / apple_texture->width;
    Vector2 apple_pos = {
        board_shape->x + apple->x * tile_size,
        board_shape->y + apple->y * tile_size,
    };
    DrawTextureEx(*apple_texture, apple_pos, 0, apple_scale, WHITE);

    // snake body
    float padding = tile_size * SCR_WORLD_PADDING / 2;
    int line_thickness = CLAMP(tile_size / 25, 3, 10);
    for (size_t i = 0; i < len(snake); i++)
    {
        size_t index = INDEX(snake, i);
        Rectangle rec = {
            board_shape->x + snake->body[index].x * tile_size + padding,
            board_shape->y + snake->body[index].y * tile_size + padding,
            tile_size - 2 * padding,
            tile_size - 2 * padding,
        };
        DrawRectangleRounded(rec, ROUNDNESS, ROUNDED_SEGMENTS, GREEN);
        DrawRectangleRoundedLinesEx(rec, ROUNDNESS, ROUNDED_SEGMENTS, line_thickness, DARKERGREEN);
    }

    // snake nose
    Vector2 head_f = {board_shape->x + snake->body[snake->head].x * tile_size,
                      board_shape->y + snake->body[snake->head].y * tile_size};
    Vector2 head_center = {head_f.x + tile_size / 2, head_f.y + tile_size / 2};
    Vector2 points[4] = {{-6, 8}, {-1.5, 2}, {0, 0}, {0, 0}};
    points[2] = (Vector2){points[0].x * -1, points[0].y};
    points[3] = (Vector2){points[1].x * -1, points[1].y};
    for (size_t i = 0; i < 4; i++)
    {
        points[i] = Vector2Add(Vector2Scale(points[i], tile_size / 20), head_f);
        points[i] = Vector2Add(points[i], (Vector2){tile_size / 2, 0});
        points[i] = rotate_about(points[i], head_center, dir_to_rad(snake->dir));
    }

    DrawLineEx(points[0], points[1], line_thickness, BROWN);
    DrawLineEx(points[2], points[3], line_thickness, BROWN);

    if (status == GS_LOST)
    {
        int str_width = MeasureText("LOST!", font_size);
        DrawText("LOST!", board_shape->x - str_width - board_shape->y / 2, board_shape->y + font_size * 1.1, font_size, ORANGE);
    }
    else if (status == GS_WIN)
    {
        int str_width = MeasureText("WON!", font_size);
        DrawText("WON!", board_shape->x - str_width - board_shape->y / 2, board_shape->y + font_size * 1.1, font_size, ORANGE);
    }

    DrawFPS(2, 2);
    EndDrawing();
}

Direction handle_input(Rectangle *board_shape, bool *restart, bool *paused, GameStatus status)
{
    bool paused_toggled = false;
    Direction dir = DIR_NONE;
    int key_pressed = GetKeyPressed();
    if (status != GS_NORMAL && key_pressed != 0)
    {
        *restart = true;
        *paused = true;
        while (GetKeyPressed())
            ;
        return dir;
    }
    do
    {
        switch (key_pressed)
        {
        case KEY_ENTER:
        case KEY_KP_ENTER:
            ToggleBorderlessWindowed();
            get_board_size(board_shape);
            break;
        case KEY_ZERO:
            *restart = true;
            break;
        case KEY_P:
            paused_toggled = true;
            *paused = !*paused;
            break;
        case KEY_W:
        case KEY_UP:
            dir = DIR_UP;
            break;
        case KEY_A:
        case KEY_LEFT:
            dir = DIR_LEFT;
            break;
        case KEY_S:
        case KEY_DOWN:
            dir = DIR_DOWN;
            break;
        case KEY_D:
        case KEY_RIGHT:
            dir = DIR_RIGHT;
            break;
        default:
            break;
        }
    } while ((key_pressed = GetKeyPressed()));
    if (!paused_toggled && dir != DIR_NONE)
        *paused = false;
    return dir;
}

void set_snake_dir(Direction cur_dir, Direction *pending_dir, Direction input_dir)
{
    if (input_dir == DIR_NONE || cur_dir == input_dir || !((cur_dir - input_dir) % 2))
        return;
    *pending_dir = input_dir;
}

void move_snake(Snake *snake)
{
    int new_head = (snake->head + 1) % SNAKE_MAX;
    if (snake->pending != DIR_NONE)
    {
        snake->dir = snake->pending;
    }
    switch (snake->dir)
    {
    case DIR_UP:
        snake->body[new_head] = (Vec2i){snake->body[snake->head].x, snake->body[snake->head].y - 1};
        break;
    case DIR_LEFT:
        snake->body[new_head] = (Vec2i){snake->body[snake->head].x - 1, snake->body[snake->head].y};
        break;
    case DIR_DOWN:
        snake->body[new_head] = (Vec2i){snake->body[snake->head].x, snake->body[snake->head].y + 1};
        break;
    case DIR_RIGHT:
        snake->body[new_head] = (Vec2i){snake->body[snake->head].x + 1, snake->body[snake->head].y};
        break;
    default:
        break;
    }
    snake->head = new_head;
    snake->tail = (snake->tail + 1) % SNAKE_MAX;
}

GameStatus check_snake(Snake *snake)
{
    if (len(snake) >= SNAKE_MAX)
        return GS_WIN;

    Vec2i head = snake->body[snake->head];
    if (head.x > BOARD_LIMIT || head.x < 0 || head.y > BOARD_LIMIT || head.y < 0)
        return GS_LOST;
    for (size_t i = 0; i < len(snake) - 1; i++)
    {
        Vec2i segment = snake->body[INDEX(snake, i)];
        if (head_equals(snake, segment))
            return GS_LOST;
    }

    return GS_NORMAL;
}

Vec2i *next(Snake *snake)
{
    static int i = 0;

    if (snake == NULL || i > len(snake))
    {
        i = 0;
        return NULL;
    }
    return &snake->body[INDEX(snake, i++)];
    // return &snake->body[(snake->tail + i++) % SNAKE_MAX];
}

static void print_vec2i(const Vec2i *vec)
{
    printf("(%d, %d)\n", vec->x, vec->y);
}

void check_apple(Snake *snake, Vec2i *apple)
{
    if (head_equals(snake, *apple))
    {
        snake->tail = snake->tail == 0 ? SNAKE_MAX - 1 : snake->tail - 1;
        *apple = get_random_point_excluding(snake, BOARD_LIMIT, BOARD_LIMIT);
    }
}

Vec2i get_random_point_excluding(Snake *snake, int max_x, int max_y)
{
    if (max_x * max_y > SNAKE_MAX)
    {
        fprintf(stderr, "get_random_point_excluding: Larger than SNAKE_MAX!\n");
        return (Vec2i){-1, -1};
    }
    Vec2i vecs[SNAKE_MAX];
    size_t end = 0;
    for (size_t x = 0; x <= max_x; x++)
    {
        for (size_t y = 0; y <= max_y; y++)
        {
            Vec2i new_vec = {x, y};
            if (!contains(snake, new_vec))
                vecs[end++] = new_vec;
        }
    }

    if (end == 0)
    {
        fprintf(stderr, "get_random_point_excluding: end=0!\n");
        return (Vec2i){-1, -1};
    }
    return vecs[GetRandomValue(0, end - 1)];
}

bool check_board_bounds(Vec2i bounds, Vec2i vec)
{
    return vec.x >= 0 && vec.x <= bounds.x && vec.y >= 0 && vec.y <= bounds.y;
}

void print_snake(Snake *snake)
{
    printf("len: %d, head: %d, tail: %d\n", len(snake), snake->head, snake->tail);
    printf("dir: %s\n", direction_name(snake->dir));

    for (int i = 0; i < len(snake); i++)
    {
        int index = INDEX(snake, i);
        printf("\t(%d, %d)\n", snake->body[index].x, snake->body[index].y);
    }
}