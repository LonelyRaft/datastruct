
#include "ncqueue.h"
#include <stdlib.h>
#include <string.h>

typedef struct nc_queue
{
    void **data;
    unsigned int size;
    unsigned int read_idx;
    unsigned int write_idx;
} ncqueue_t;

ncqueue_t *nc_queue_create(const unsigned int _length)
{
    if (0 == _length)
        return 0;

    unsigned int size = sizeof(ncqueue_t) + sizeof(void *) * _length;
    ncqueue_t *queue = (ncqueue_t *)malloc(size);
    if (queue)
    {
        memset(queue, 0, size);
        queue->size = _length;
        queue->read_idx = 0;
        queue->write_idx = 0;
        queue->data = (void **)(queue + 1);
    }
    return queue;
}

int nc_queue_destroy(ncqueue_t *_queue)
{
    free(_queue);
}

void *nc_queue_pop(ncqueue_t *_queue)
{
    if (0 == _queue)
        return 0;

    if (_queue->read_idx == _queue->write_idx)
        return 0;

    void *top = _queue->data[_queue->read_idx];
    _queue->read_idx++;
    if (_queue->read_idx == _queue->size)
        _queue->read_idx = 0;
    return top;
}

void *nc_queue_top(ncqueue_t *_queue)
{
    if (0 == _queue)
        return 0;

    if (_queue->read_idx == _queue->write_idx)
        return 0;

    return _queue->data[_queue->read_idx];
}

int nc_queue_push(ncqueue_t *_queue, void *_data)
{
    if (0 == _queue || 0 == _data)
        return -1;

    unsigned int widx;
    widx = _queue->write_idx + 1;
    if (widx == _queue->size)
        widx = 0;
    if (widx == _queue->read_idx)
        return -2;

    _queue->data[_queue->write_idx] = _data;
    _queue->write_idx = widx;
    return 0;
}

int nc_queue_full(ncqueue_t *_queue)
{
    if (0 == _queue)
        return -1;
    unsigned int widx = _queue->write_idx + 1;
    if (widx == _queue->size)
        widx = 0;
    return widx == _queue->read_idx;
}

int nc_queue_empty(ncqueue_t *_queue)
{
    if (0 == _queue)
        return -1;
    return _queue->read_idx == _queue->write_idx;
}

int nc_queue_clear(ncqueue_t *_queue)
{
    if (0 == _queue)
        return -1;
    _queue->read_idx = 0;
    _queue->write_idx = 0;
    return 0;
}
