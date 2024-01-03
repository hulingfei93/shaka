#include <sse_core.h>

static sse_tstree_node_t *sse_tstree_node_alloc(sse_tstree_t *tree, char c);

sse_tstree_t *
sse_tstree_create(sse_pool_t *pool, sse_log_t *log)
{
    sse_tstree_t *tree;

    tree = (sse_tstree_t *)sse_pool_alloc(pool, sizeof(sse_tstree_t));
    if (!tree) {
        return SSE_NULL;
    }

    tree->root = SSE_NULL;
    tree->word_count = 0;
    tree->node_count = 0;
    tree->pool = pool;
    tree->log = log;

    return tree;
}

void
sse_tstree_insert(sse_tstree_t *tree, sse_str_t *key, void *data)
{
    size_t              i;
    char                *s;
    sse_tstree_node_t   *cur;

    if (key->len == 0) {
        return;
    }
    
    i = 0;
    s = key->data;

    if (!tree->root) {
        tree->root = sse_tstree_node_alloc(tree, *s);
    }

    cur = tree->root;
    while (1) {
        if (*s == cur->split_char) {
            ++s;
            if (++i == key->len) {
                cur->data = data;
                ++tree->word_count;
                return;
            }
            if (!cur->middle) {
                cur->middle = sse_tstree_node_alloc(tree, *s);
            }
            cur = cur->middle;
        }
        else if (*s < cur->split_char) {
            if( !cur->left) {
                cur->left = sse_tstree_node_alloc(tree, *s);
            }
            cur = cur->left;
        }
        else {
            if (!cur->right) {
                cur->right = sse_tstree_node_alloc(tree, *s);
            }
            cur = cur->right;
        }
    }

    return;
}

static sse_tstree_node_t *
sse_tstree_node_alloc(sse_tstree_t *tree, char c)
{
    sse_tstree_node_t *new;

    new = (sse_tstree_node_t *)sse_pool_alloc(tree->pool, sizeof(sse_tstree_node_t));
    new->split_char = c;
    new->data = new->left = new->right = new->middle = SSE_NULL;

    ++tree->node_count;
    return new;
}

void *
sse_tstree_search(sse_tstree_t *tree, sse_str_t *key)
{
    size_t              i;
    char                *s;
    sse_tstree_node_t   *cur;

    if (key->len == 0) {
        return SSE_NULL;
    }

    i = 0;
    s = key->data;
    cur = tree->root;

    while (1) {
        if (!cur) {
            return SSE_NULL;
        }
        if (*s == cur->split_char) {
            ++s;
            if (++i == key->len) {
                return cur->data;
            }
            cur = cur->middle;
        }
        else if (*s < cur->split_char) {
            cur = cur->left;
        }
        else {
            cur = cur->right;
        }
    }

    return SSE_NULL;
}

