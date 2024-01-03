#ifndef _SLIST_H_
#define _SLIST_H_

#include <assert.h>

template<class T> class SList;

template<class T>
class SList // singly linked list with head node
{
private:
	struct Node
	{
		T data;
		Node *next;
		Node() { next = 0; }
		Node(const T& a, Node *p = 0) { data = a; next = p; }
	};
public:
	class Iterator;
	friend class Iterator;
	class Iterator
	{
		friend SList;
	public:
		Iterator() { nodePtr = 0; }
		Iterator(const Iterator& iterator) { nodePtr = iterator.nodePtr; }
		Iterator& operator=(const Iterator& rhs)
		{
			if (this != &rhs)
			{
				nodePtr = rhs.nodePtr;
			}
			return *this;

		}
		T& operator*() const
		{
			assert(nodePtr != 0);
			return nodePtr->data;
		}
		Iterator& operator++()
		{
			assert(nodePtr != 0);
			nodePtr = nodePtr->next;
			return *this;
		}
		Iterator operator++(int)
		{
			assert(nodePtr != 0);
			Iterator oldIterator = *this;
			++(*this);
			return oldIterator;
		}
		bool operator==(const Iterator& iter)
		{
			return nodePtr == iter.nodePtr;
		}
		bool operator!=(const Iterator& iter)
		{
			return !(*this == iter);
		}
	private:
		Node *nodePtr;
		Iterator(Node *nodePointer) { nodePtr = nodePointer; }
		
	};
private:
	Node *head, *tail;

public:
	SList();
	SList(const SList<T>&);
	SList<T>& operator=(const SList<T>&);
	~SList();
	
	Iterator Begin()
	{
		Iterator iter(head->next);
		return iter;
	}
	Iterator End()
	{
		Iterator iter(0);
		return iter;
	}
	bool IsEmpty() const;
	const T& ShowFirst() const;

	void AddFirst(const T&);
	void AddLast(const T&);
	void AddAfter(Iterator, const T&);
	void RemoveFirst();
	void RemoveAfter(Iterator);
	void SetToEmpty();
};

template <class T>
SList<T>::SList()
{
	head = new Node(); // head node for list
	tail = 0;
}

template <class T>
SList<T>::SList(const SList<T> & cObj)
{
	head = new Node();
	tail = 0;
	Node* currentNode = cObj.head->next;
	while (currentNode != 0)
	{
		AddLast(currentNode->data);
		currentNode = currentNode->next;
	}
}

template <class T>
SList<T>& SList<T>::operator=(const SList<T> &rhs)
{
	if (this != &rhs)
	{
		SetToEmpty();
		Node* currentNode = rhs.head->next;
		while (currentNode != 0)
		{
			AddLast(currentNode->data);
			currentNode = currentNode->next;
		}
	}
	return *this;
}

template <class T>
SList<T>::~SList()
{
	SetToEmpty();
	delete head;
}

template <class T>
bool SList<T>::IsEmpty() const
{
	return head->next == 0;
}

template <class T>
const T& SList<T>::ShowFirst() const
{
	assert(!IsEmpty());
	return head->next->data;
}

template <class T>
void SList<T>::AddFirst(const T &newItem)
{
	Node *newNode = new Node(newItem, head->next);
	head->next = newNode;
	if (newNode->next == 0)
		tail = newNode;
}

template <class T>
void SList<T>::AddLast(const T &newItem)
{
	if (IsEmpty())
		return AddFirst(newItem);
	else
	{
		Node *newNode = new Node(newItem);
		tail->next = newNode;
		tail = newNode;
	}
}

template <class T>
void SList<T>::AddAfter(Iterator i, const T &newItem)
{
	assert(i.nodePtr != 0);
	Node *newNode = new Node(newItem, i.nodePtr->next);
	if (i.nodePtr->next == 0) // iterator points to last node of list
		tail = newNode;
	i.nodePtr->next = newNode;
}

template <class T>
void SList<T>::RemoveFirst()
{
	assert(!IsEmpty());
	Node *firstNode = head->next;
	head->next = firstNode->next;
	if (IsEmpty()) // empty after remove
		tail = 0;
	delete firstNode;
}

template <class T>
void SList<T>::RemoveAfter(Iterator i)
{
	assert(i.nodePtr != 0 && i.nodePtr->next != 0);
	Node *deleteNode = i.nodePtr->next;
	i.nodePtr->next = i.nodePtr->next->next;
	delete deleteNode;
	if (i.nodePtr->next == 0) // remove last node of list
		tail = i.nodePtr;
}

template <class T>
void SList<T>::SetToEmpty()
{
	while (!IsEmpty())
		RemoveFirst();
}

#endif