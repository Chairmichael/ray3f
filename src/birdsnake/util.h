#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include "raylib.h"
#include "raymath.h"

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
    size_t capacity, head, count;
} RingBuffer;

static inline bool rb_is_full(RingBuffer rb)
{
    return rb.capacity == rb.count;
}

RingBuffer create_ring_buffer(size_t capacity);
void free_ring_buffer(RingBuffer *rb);
Vector2 rb_peek_head(RingBuffer rb);
Vector2 rb_peek_tail(RingBuffer rb);
Vector2 rb_peek_element(const RingBuffer *rb, size_t i);
bool rb_pop_tail(RingBuffer *rb, Vector2 *out);
bool rb_pop_head(RingBuffer *rb, Vector2 *out);
bool rb_append(RingBuffer *rb, Vector2 vec);
bool rb_prepend(RingBuffer *rb, Vector2 vec);
bool rb_contains(RingBuffer rb, Vector2 vec);

#endif