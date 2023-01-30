
#ifndef _NC_RBTREE_H
#define _NC_RBTREE_H

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct rbtree_ifs
    {
        int (*cmp)(const void *_key1, const void *_key2);
    } rbtree_ifs_t;

    typedef struct nc_rbtree ncrbtree_t;

#ifdef __cplusplus
}
#endif

#endif // _NC_RBTREE_H
