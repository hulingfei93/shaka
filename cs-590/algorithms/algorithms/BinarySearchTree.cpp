#include "BinarySearchTree.h"

BinarySearchTree::BinarySearchTree()
{
	rootPtr = 0;
}

BinarySearchTree::~BinarySearchTree()
{
	while (!isEmpty())
		deleteMin();
}

bool BinarySearchTree::search(int value) const
{
	NodeType *c = rootPtr;
	while (c != 0)
	{
		if (c->value > value)
			c = c->leftPtr;
		else if (c->value < value)
			c = c->rightPtr;
		else
			return true;
	}
	return false;
}

void BinarySearchTree::insert(int value)
{
	NodeType *p = 0;
	NodeType *c = rootPtr;
	NodeType *newNode = new NodeType();
	newNode->value = value;
	newNode->leftPtr = 0;
	newNode->rightPtr = 0;

	while (c != 0)
	{
		p = c;
		if (c->value >= value)
			c = c->leftPtr;
		else
			c = c->rightPtr;
	}

	if (!p)
		rootPtr = newNode;
	else
	{
		if (p->value >= value)
			p->leftPtr = newNode;
		else
			p->rightPtr = newNode;
	}
}

int BinarySearchTree::deleteMin()
{
	NodeType *p = 0;
	NodeType *c = rootPtr;
	int returnValue;

	while (c->leftPtr != 0)
	{
		p = c;
		c = c->leftPtr;
	}
	returnValue = c->value;
	if (p == 0) // delete root node
	{
		rootPtr = c->rightPtr;
	}
	else // delete non-root node
	{ 
		p->leftPtr = c->rightPtr;
	}
	delete c;

	return returnValue;
	
}

bool BinarySearchTree::isEmpty() const
{
	return rootPtr == 0;
}