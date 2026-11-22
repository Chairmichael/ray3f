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
    size_t size;
    size_t head;
    size_t tail;
    bool isfull;
} RingBuffer;

RingBuffer create_ring_buffer(size_t size);
void free_ring_buffer(RingBuffer rb);
Vector2 peek_head_rb(RingBuffer rb);
Vector2 peek_tail_rb(RingBuffer rb);
Vector2 peek_element_rb(RingBuffer rb, size_t i);
Vector2 pop_tail_rb(RingBuffer *rb);
Vector2 pop_head_rb(RingBuffer *rb);
bool append_rb(RingBuffer *rb, Vector2 vec);
bool prepend_rb(RingBuffer *rb, Vector2 vec);

#endif