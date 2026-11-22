#include "util.h"

RingBuffer create_ring_buffer(size_t size)
{
    RingBuffer rb = {0};
    rb.arr = MemAlloc(sizeof(*rb.arr) * size);
    rb.size = size;
    return rb;
}

void free_ring_buffer(RingBuffer rb)
{
    MemFree(rb.arr);
}

Vector2 peek_head_rb(RingBuffer rb)
{
    return rb.arr[rb.head];
}

Vector2 peek_tail_rb(RingBuffer rb)
{
    return rb.arr[rb.tail];
}

// Returns element i places back from the head
Vector2 peek_element_rb(RingBuffer rb, size_t i)
{
    return rb.arr[(rb.head - i) % rb.size];
}

Vector2 pop_tail_rb(RingBuffer *rb)
{
    if (rb->isfull || rb->head != rb->tail)
    {
        Vector2 tail_element = peek_tail_rb(*rb);
        rb->tail = (rb->tail + 1) % rb->size;
        rb->isfull = false;
        return tail_element;
    }
    return (Vector2){-1, -1};
}

Vector2 pop_head_rb(RingBuffer *rb)
{
    if (rb->isfull || rb->head != rb->tail)
    {
        Vector2 head_element = peek_head_rb(*rb);
        rb->head = rb->head == 0 ? rb->size - 1 : rb->head - 1;
        rb->isfull = false;
        return head_element;
    }
    return (Vector2){-1, -1};
}

// Add element to tail (new tail)
bool append_rb(RingBuffer *rb, Vector2 vec)
{
    if (rb->isfull)
        return false;
    rb->tail = rb->tail == 0 ? rb->size - 1 : rb->tail - 1;
    rb->arr[rb->tail] = vec;
    if (rb->head == rb->tail)
        rb->isfull = true;
    return true;
}

// Add element to head (new head)
bool prepend_rb(RingBuffer *rb, Vector2 vec)
{
    if (rb->isfull)
        return false;
    rb->head = (rb->head + 1) % rb->size;
    rb->arr[rb->head] = vec;
    if (rb->head == rb->tail)
        rb->isfull = true;
    return true;
}
