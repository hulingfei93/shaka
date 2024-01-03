#include "PriorityQueue.h"

PriorityQueue::PriorityQueue(int capacity)
{
	assert(capacity >= 0);
	this->capacity = capacity;
	this->tailIndex = -1;
	array = new int[capacity];
}

PriorityQueue::~PriorityQueue()
{
	this->capacity = 0;
	this->tailIndex = -1;
	delete[] array;
}

bool PriorityQueue::isEmpty() const
{
	return tailIndex == -1;
}

int PriorityQueue::getSize() const
{
	return tailIndex + 1;
}

void PriorityQueue::insert(int newItem)
{
	assert(tailIndex + 1 < capacity);
	array[++tailIndex] = newItem;
	minHeapifyFromBottom(tailIndex);
}

void PriorityQueue::minHeapifyFromBottom(int i)
{
	int cIndex, pIndex, temp;
	cIndex = i;
	for (pIndex = parentIndex(cIndex); 
		cIndex > 0 && pIndex >= 0 && array[cIndex] < array[pIndex]; 
		cIndex = pIndex, pIndex = parentIndex(cIndex))
	{
		temp = array[cIndex];
		array[cIndex] = array[pIndex];
		array[pIndex] = temp;
	}
}

void PriorityQueue::minHeapifyFromTop(int i)
{
	int cIndex, lIndex, rIndex, leastIndex, temp;
	leastIndex = cIndex = i;
	
	while (true)
	{
		lIndex = leftIndex(cIndex);
		rIndex = rightIndex(cIndex);
		if (lIndex <= tailIndex && array[lIndex] < array[leastIndex])
			leastIndex = lIndex;
		if (rIndex <= tailIndex && array[rIndex] < array[leastIndex])
			leastIndex = rIndex;

		if (leastIndex == cIndex)
			break;
		else
		{
			temp = array[cIndex];
			array[cIndex] = array[leastIndex];
			array[leastIndex] = temp;
		}
		cIndex = leastIndex;
	}
}

int PriorityQueue::deleteMin()
{
	assert(tailIndex >= 0);
	int min = array[0];
	array[0] = array[tailIndex--];
	minHeapifyFromTop(0);

	return min;

}

int PriorityQueue::leftIndex(int i) const
{
	return 2 * i + 1;
}

int PriorityQueue::rightIndex(int i) const
{
	return 2 * i + 2;
}

int PriorityQueue::parentIndex(int i) const
{
	return (i - 1) / 2;
}