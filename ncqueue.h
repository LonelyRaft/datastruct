
#ifndef _NC_QUEUE_H
#define _NC_QUEUE_H

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct nc_queue ncqueue_t;

    ncqueue_t *nc_queue_create(const unsigned int _length);

    int nc_queue_destroy(ncqueue_t *_queue);

    void *nc_queue_pop(ncqueue_t *_queue);

    void *nc_queue_top(ncqueue_t *_queue);

    int nc_queue_push(ncqueue_t *_queue, void *_data);

    // 1 is true, 0 is false, -1 is error
    int nc_queue_full(ncqueue_t *_queue);

    // 1 is true, 0 is false, -1 is error
    int nc_queue_empty(ncqueue_t *_queue);

    int nc_queue_clear(ncqueue_t *_queue);

#ifdef __cplusplus
}
#endif

#endif // _NC_QUEUE_H
