#include <sse_core.h>

static void sse_rbtree_left_rotate(sse_rbtree_t *tree, sse_rbtree_node_t *node);
static void sse_rbtree_right_rotate(sse_rbtree_t *tree, sse_rbtree_node_t *node);
static void sse_rbtree_insert_fixup(sse_rbtree_t *tree, sse_rbtree_node_t *node);

sse_rbtree_t *
sse_rbtree_create(sse_pool_t *pool, sse_log_t *log)
{
    sse_rbtree_t *tree;

    tree = (sse_rbtree_t *)sse_pool_alloc(pool, sizeof(sse_rbtree_t));
    if (!tree) {
        return SSE_NULL;
    }

    tree->sentinel = (sse_rbtree_node_t *)sse_pool_calloc(pool, sizeof(sse_rbtree_node_t));
    if (!tree->sentinel) {
        return SSE_NULL;
    }
    SSE_RBTREE_BLACK(tree->sentinel);

    tree->root = tree->sentinel;
    tree->count = 0;
    tree->pool = pool;
    tree->log = log;

    return tree;
}

void
sse_rbtree_insert(sse_rbtree_t *tree, sse_str_t *key, void *value)
{
    sse_rbtree_node_t *p, *c, *new_node, *sentinel;
    sse_int_t cmp_result;

    sentinel = tree->sentinel;
    p = sentinel;
    c = tree->root;

    while (c != sentinel) {
        p = c;

        cmp_result = sse_str_strcmp(key, c->key);
        if (cmp_result < 0) {
            c = c->left;
        }
        else if (cmp_result > 0) {
            c = c->right;
        }
        else {                      /* for existing key, omit and return directly */
            return;
        }
    }

    new_node = (sse_rbtree_node_t *)sse_pool_alloc(tree->pool, sizeof(sse_rbtree_node_t));
    new_node->key =  key;
    new_node->value = value;
    new_node->left = sentinel;
    new_node->right = sentinel;
    new_node->parent = p;
    SSE_RBTREE_RED(new_node);

    if (p == sentinel) {
        tree->root = new_node;
    }
    else if (sse_str_strcmp(new_node->key, p->key) < 0) {
        p->left = new_node;
    }
    else {
        p->right = new_node;
    }
    ++tree->count;
    sse_rbtree_insert_fixup(tree, new_node);
}

static void
sse_rbtree_insert_fixup(sse_rbtree_t *tree, sse_rbtree_node_t *node)
{
    sse_rbtree_node_t *uncle_node;

    while (SSE_RBTREE_IS_RED(node->parent)) {
        if (node->parent == node->parent->parent->left) {
            uncle_node = node->parent->parent->right;

            /* node's uncle is red */
            if (SSE_RBTREE_IS_RED(uncle_node)) {                
                SSE_RBTREE_BLACK(node->parent);
                SSE_RBTREE_BLACK(uncle_node);
                SSE_RBTREE_RED(node->parent->parent);
                node = node->parent->parent;
            }
            /* node's uncle is black and node is a right child */
            else if (node == node->parent->right) {
                node = node ->parent;
                sse_rbtree_left_rotate(tree, node);
            }
            /* node's uncle is black and node is a left child */
            else {
                SSE_RBTREE_BLACK(node->parent);
                SSE_RBTREE_RED(node->parent->parent);
                sse_rbtree_right_rotate(tree, node->parent->parent);
            }
        }
        else {
            uncle_node = node->parent->parent->left;

            /* node's uncle is red */
            if (SSE_RBTREE_IS_RED(uncle_node)) {                
                SSE_RBTREE_BLACK(node->parent);
                SSE_RBTREE_BLACK(uncle_node);
                SSE_RBTREE_RED(node->parent->parent);
                node = node->parent->parent;
            }
            /* node's uncle is black and node is a left child */
            else if (node == node->parent->left) {
                node = node ->parent;
                sse_rbtree_right_rotate(tree, node);
            }
            /* node's uncle is black and node is a right child */
            else {
                SSE_RBTREE_BLACK(node->parent);
                SSE_RBTREE_RED(node->parent->parent);
                sse_rbtree_left_rotate(tree, node->parent->parent);
            }
        }
    }
    SSE_RBTREE_BLACK(tree->root);
}

sse_bool_t
sse_rbtree_contains(sse_rbtree_t *tree, sse_str_t *key)
{
    sse_rbtree_node_t *c, *sentinel;
    sse_int_t cmp_result;

    c = tree->root;
    sentinel = tree->sentinel;

    while (c != sentinel) {
        cmp_result = sse_str_strcmp(key, c->key);
        if (cmp_result < 0) {
            c = c->left;
        }
        else if (cmp_result > 0) {
            c = c->right;
        }
        else {
            return SSE_BOOL_TRUE;
        }
    }

    return SSE_BOOL_FALSE;
}

void *
sse_rbtree_find(sse_rbtree_t *tree, sse_str_t *key)
{
    sse_rbtree_node_t *c, *sentinel;
    sse_int_t cmp_result;

    c = tree->root;
    sentinel = tree->sentinel;

    while (c != sentinel) {
        cmp_result = sse_str_strcmp(key, c->key);
        if (cmp_result < 0) {
            c = c->left;
        }
        else if (cmp_result > 0) {
            c = c->right;
        }
        else {
            return c->value;
        }
    }

    return SSE_NULL;
}

static void
sse_rbtree_inorder_imp(sse_rbtree_node_t *node, sse_rbtree_node_t *sentinel)
{
    if (node == sentinel) {
        return;
    }

    sse_rbtree_inorder_imp(node->left, sentinel);
    write(SSE_FD_STDOUT, node->key->data, node->key->len);
    write(SSE_FD_STDOUT, "\n", 1);
    sse_rbtree_inorder_imp(node->right, sentinel);
    
}

void
sse_rbtree_inorder(sse_rbtree_t *tree)
{
    sse_rbtree_inorder_imp(tree->root, tree->sentinel);
}

static void
sse_rbtree_left_rotate(sse_rbtree_t *tree, sse_rbtree_node_t *node)
{
    sse_rbtree_node_t *temp;

    temp = node->right;
    node->right = temp->left;
    
    temp->left->parent = node;

    temp->parent = node->parent;

    if (node->parent == tree->sentinel) {   /* node is root */
        tree->root = temp;
    }
    else if (node == node->parent->left) {  /* node is the left child of its parent */
        node->parent->left = temp;
    }
    else {                                  /* node is the right child of its parent */ 
        node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;
}

static void
sse_rbtree_right_rotate(sse_rbtree_t *tree, sse_rbtree_node_t *node)
{
    sse_rbtree_node_t *temp;

    temp = node->left;
    node->left = temp->right;
    
    temp->right->parent = node;

    temp->parent = node->parent;

    if (node->parent == tree->sentinel) {   /* node is root */
        tree->root = temp;
    }
    else if (node == node->parent->left) {  /* node is the left child of its parent */
        node->parent->left = temp;
    }
    else {                                  /* node is the right child of its parent */ 
        node->parent->right = temp;
    }

    temp->right = node;
    node->parent = temp;
}
