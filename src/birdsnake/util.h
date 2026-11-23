#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include "raylib.h"

typedef enum
{
    GS_PAUSED,
    GS_NORMAL,
    GS_IDLE,
    GS_LOST,
    GS_WON,
} GameState;

typedef struct
{
    Vector2 *arr;
    size_t size, head, count;
    bool isfull;
} RingBuffer;

RingBuffer create_ring_buffer(size_t size);
void free_ring_buffer(RingBuffer *rb);
Vector2 peek_head_rb(RingBuffer rb);
Vector2 peek_tail_rb(RingBuffer rb);
Vector2 peek_element_rb(const RingBuffer *rb, size_t i);
bool pop_tail_rb(RingBuffer *rb, Vector2 *out);
bool pop_head_rb(RingBuffer *rb, Vector2 *out);
bool append_rb(RingBuffer *rb, Vector2 vec);
bool prepend_rb(RingBuffer *rb, Vector2 vec);

#endif