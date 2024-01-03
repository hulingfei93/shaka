#ifndef _BINARY_SEARCH_TREE_H_
#define _BINARY_SEARCH_TREE_H_

#include <assert.h>

typedef struct BinarySearchTreeNode
{
    int value;
    BinarySearchTreeNode *leftPtr;
    BinarySearchTreeNode *rightPtr;
} NodeType;

class BinarySearchTree
{
public:
    BinarySearchTree();
    ~BinarySearchTree();
    bool search(int value) const;
    void insert(int value);
    int deleteMin();
    bool isEmpty() const;

private:
    NodeType *rootPtr;
};

#endif