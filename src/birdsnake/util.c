#include "util.h"

static size_t tail_idx(const RingBuffer *rb)
{
    return (rb->head + rb->capacity - (rb->count - 1)) % rb->capacity; // only valid if count > 0
}

RingBuffer create_ring_buffer(size_t capacity)
{
    RingBuffer rb = {0};
    rb.arr = MemAlloc(sizeof(*rb.arr) * capacity);
    rb.capacity = capacity;
    return rb;
}

void free_ring_buffer(RingBuffer *rb)
{
    MemFree(rb->arr);
    rb = NULL;
}

Vector2 rb_peek_head(RingBuffer rb)
{
    return rb.arr[rb.head];
}

Vector2 rb_peek_tail(RingBuffer rb)
{
    return rb.arr[tail_idx(&rb)];
}

bool rb_prepend(RingBuffer *rb, Vector2 v)
{
    if (rb->count == rb->capacity)
        return false;
    rb->head = (rb->head + 1) % rb->capacity;
    rb->arr[rb->head] = v;
    rb->count++;
    return true;
}

bool rb_append(RingBuffer *rb, Vector2 v)
{
    if (rb->count == rb->capacity)
        return false;
    if (rb->count == 0)
        return rb_prepend(rb, v);
    size_t t = (tail_idx(rb) + rb->capacity - 1) % rb->capacity;
    rb->arr[t] = v;
    rb->count++;
    return true;
}

bool rb_pop_tail(RingBuffer *rb, Vector2 *out)
{
    if (rb->count == 0)
        return false;
    *out = rb->arr[tail_idx(rb)];
    rb->count--;
    return true;
}

bool rb_pop_head(RingBuffer *rb, Vector2 *out)
{
    if (rb->count == 0)
        return false;
    *out = rb->arr[rb->head];
    rb->head = (rb->head + rb->capacity - 1) % rb->capacity;
    rb->count--;
    return true;
}

Vector2 rb_peek_element(const RingBuffer *rb, size_t i) // i < count
{
    return rb->arr[(rb->head + rb->capacity - i) % rb->capacity];
}

bool rb_contains(RingBuffer rb, Vector2 vec)
{
    for (size_t i = 0; i < rb.count; i++)
    {
        if (Vector2Equals(rb_peek_element(&rb, i), vec))
            return true;
    }
    return false;
}
