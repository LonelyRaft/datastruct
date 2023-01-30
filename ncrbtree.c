
#include "ncrbtree.h"
#include "ncqueue.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define RBTREE_RED 0x00
#define RBTREE_BLACK 0x01
typedef struct nc_rbtree_node
{
    void *data;
    unsigned int color;
    struct nc_rbtree_node *left;
    struct nc_rbtree_node *right;
    struct nc_rbtree_node *parent;
} node_t;

typedef struct node_pool
{
    node_t *pool;
    ncqueue_t *queue;
    pthread_mutex_t mutex;
} pool_t;
static pool_t *g_pool = 0;

static node_t *node_lease(void)
{
    if (0 == g_pool)
        return 0;
    pthread_mutex_lock(&g_pool->mutex);
    void *data = nc_queue_pop(g_pool->queue);
    pthread_mutex_unlock(&g_pool->mutex);
    return (node_t *)data;
}

static int node_release(node_t *_node)
{
    if (0 == _node)
        return 0;
    if (0 == g_pool)
        return -1;
    pthread_mutex_lock(&g_pool->mutex);
    int result = nc_queue_push(g_pool->queue, _node);
    pthread_mutex_unlock(&g_pool->mutex);
    return result;
}

int nc_rbtree_init(const unsigned int _length)
{
    if (g_pool)
        return 0;

    if (0 == _length)
        return -1;

    unsigned int size = sizeof(pool_t) + sizeof(node_t) * _length;
    g_pool = (pool_t *)malloc(size);
    if (0 == g_pool)
        return -2;

    pthread_mutex_init(&g_pool->mutex, 0);

    g_pool->queue = nc_queue_create(_length + 1);
    if (0 == g_pool)
    {
        free(g_pool);
        return -2;
    }

    g_pool->pool = (node_t *)(g_pool + 1);
    for (int idx = 0; idx < _length; idx++)
        nc_queue_push(g_pool->queue, g_pool->pool + idx);
    return 0;
}

typedef struct nc_rbtree
{
    node_t *root;
    const rbtree_ifs_t *ifs;
} ncrbtree_t;

// left rotate

// right rotate

// insert

// remove

// sort



