#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include <assert.h>

class PriorityQueue
{
public:
    PriorityQueue(int capacity);
    ~PriorityQueue();
    bool isEmpty() const;
    int getSize() const;
    void insert(int newItem);
    int deleteMin();
private:
    int leftIndex(int i) const;
    int rightIndex(int i) const;
    int parentIndex(int i) const;
    void minHeapifyFromBottom(int i);
    void minHeapifyFromTop(int i);
    int capacity;
    int tailIndex; 
    int *array;
};

#endif