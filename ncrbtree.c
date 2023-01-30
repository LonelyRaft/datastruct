
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
    node_t *data = (node_t *)
        nc_queue_pop(g_pool->queue);
    pthread_mutex_unlock(&g_pool->mutex);
    if (data)
    {
        data->color = RBTREE_RED;
        data->data = 0;
        data->left = data->right = data->parent = 0;
    }
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

static node_t *node_min(const node_t *_root)
{
    if (0 == _root)
        return 0;
    node_t *node = _root;
    while (node->left)
        node = node->left;
    return node;
}

static node_t *node_max(const node_t *_root)
{
    if (0 == _root)
        return 0;
    node_t *node = _root;
    while (node->right)
        node = node->right;
    return node;
}

static node_t *node_next(const node_t *_node)
{
    if (0 == _node)
        return 0;

    if (_node->right)
        return node_min(_node->right);

    node_t *node = _node->parent;
    if (0 == node)
        return 0;
    while (node->parent)
    {
        if (node == node->parent->left)
            break;
        node = node->parent;
    }
    return node;
}

static node_t *node_prev(const node_t *_node)
{
    if (0 == _node)
        return 0;

    if (_node->left)
        return node_max(_node);

    node_t *node = _node->parent;
    if (0 == node)
        return 0;
    while (node->parent)
    {
        if (node == node->parent->right)
            break;
        node = node->parent;
    }
    return node;
}

static node_t *node_left_rotate(node_t *_node)
{
    node_t *root = _node;
    if (0 == _node)
        return root;

    node_t *right = _node->right;
    if (0 == _node->right)
        return root;
    if (0 == _node->parent)
        root = right;
    else if (_node == _node->parent->left)
        _node->parent->left = right;
    else // if (_node == _node->parent->right)
        _node->parent->right = right;
    right->parent = _node->parent;

    _node->right = right->left;
    if (right->left)
        right->left->parent = _node;

    right->left = _node;
    _node->parent = right;

    return root;
}

static node_t *node_right_rotate(node_t *_node)
{
    node_t *root = _node;
    if (0 == _node)
        return root;

    node_t *left = _node->left;
    if (0 == _node->left)
        return root;
    if (0 == _node->parent)
        root = left;
    else if (_node == _node->parent->left)
        _node->parent->left = left;
    else // if (_node == _node->parent->right)
        _node->parent->right = left;
    left->parent = _node->parent;

    _node->left = left->right;
    if (left->right)
        left->right->parent = _node;

    left->right = _node;
    _node->parent = left;

    return root;
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

// search
void *rbtree_search(ncrbtree_t *_tree, void *_key_data)
{
    if (0 == _key_data ||
        0 == _tree ||
        0 == _tree->root)
        return 0;

    node_t *node = _tree->root;
    while (node)
    {
        int result =
            _tree->ifs->cmp(
                node->data, _key_data);
        if (0 > result)
            node = node->right;
        else if (0 < result)
            node = node->left;
        else
            break;
    }

    if (0 == node)
        return 0;
    return node->data;
}

// insert
int rbtree_insert(
    ncrbtree_t *_tree,
    const void *_data)
{
    if (0 == _tree || 0 == _data)
        return -1;

    node_t *node = node_lease();
    if (0 == node)
        return -2;
    node->data = _data;

    if (0 == _tree->root)
    {
        node->color = RBTREE_BLACK;
        _tree->root = node;
        return 0;
    }

    node_t *curr = _tree->root;
    while (curr)
    {
        int result =
            _tree->ifs->cmp(
                curr->data, _data);
        if (0 == result)
            return 0;
        else if (0 > result)
        {
            if (curr->right)
                curr = curr->right;
            else
            {
                node->parent = curr;
                curr->right = node;
                break;
            }
        }
        else
        {
            if (curr->left)
                curr = curr->left;
            else
            {
                node->parent = curr;
                curr->left = node;
                break;
            }
        }
    }
    return 0;
}

// remove
int rbtree_remove(
    ncrbtree_t *_tree,
    const void *_data)
{
    
    return 0;
}
// sort
