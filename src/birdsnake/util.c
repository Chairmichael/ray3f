#include "util.h"

RingBuffer create_ring_buffer(size_t size)
{
    RingBuffer rb = {0};
    rb.arr = MemAlloc(sizeof(*rb.arr) * size);
    rb.size = size;
    return rb;
}

void free_ring_buffer(RingBuffer *rb)
{
    MemFree(rb->arr);
    rb = NULL;
}

Vector2 peek_head_rb(RingBuffer rb)
{
    return rb.arr[rb.head];
}

Vector2 peek_tail_rb(RingBuffer rb)
{
    return rb.arr[tail_idx(&rb)];
}

static size_t tail_idx(const RingBuffer *rb)
{
    return (rb->head + rb->size - (rb->count - 1)) % rb->size; // only valid if count > 0
}

bool prepend_rb(RingBuffer *rb, Vector2 v)
{
    if (rb->count == rb->size)
        return false;
    rb->head = (rb->head + 1) % rb->size;
    rb->arr[rb->head] = v;
    rb->count++;
    return true;
}

bool append_rb(RingBuffer *rb, Vector2 v)
{
    if (rb->count == rb->size)
        return false;
    if (rb->count == 0)
        return prepend_rb(rb, v);
    size_t t = (tail_idx(rb) + rb->size - 1) % rb->size;
    rb->arr[t] = v;
    rb->count++;
    return true;
}

bool pop_tail_rb(RingBuffer *rb, Vector2 *out)
{
    if (rb->count == 0)
        return false;
    *out = rb->arr[tail_idx(rb)];
    rb->count--;
    return true;
}

bool pop_head_rb(RingBuffer *rb, Vector2 *out)
{
    if (rb->count == 0)
        return false;
    *out = rb->arr[rb->head];
    rb->head = (rb->head + rb->size - 1) % rb->size;
    rb->count--;
    return true;
}

Vector2 peek_element_rb(const RingBuffer *rb, size_t i) // i < count
{
    return rb->arr[(rb->head + rb->size - i) % rb->size];
}