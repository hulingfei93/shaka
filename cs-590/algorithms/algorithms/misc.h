#include <assert.h>
#include <iostream>
#include "SList.h"
#include "List.h"
#include "Stack.h"
#include "Queue.h"

using namespace std;

int findMissingElement(SList<int>&, int);
int binarySearch(int[], int);
void transerFromQueueToStack(Queue<int>&, Stack<int>&);
int factorial(int);
int computeNumberOfCombination(int, int);
void reverseArrayResursive(int[], int);
void reverseArrayResursiveImp(int[], int, int);
int binarySearchResursive(int[], int, int);
int binarySearchResursiveImp(int[], int, int, int);
void shiftArrayCounterclockwise(int[], int, int);
void reverseArray(int[], int, int);

void misc_test()
{
	SList<int> list1;
	list1.AddFirst(5);
	list1.AddFirst(3);
	list1.AddFirst(0);
	list1.AddFirst(1);
	list1.AddFirst(2);
	cout << findMissingElement(list1, 5) << endl;

	cout << "***********************************************" << endl;

	list list2;
	list2.insert(1);
	list2.insert(2);
	list2.insert(3);
	list2.insert(4);
	list2.insert(5);
	list2.print();
	list2.reverse();
	list2.print();
	list2.reverse_resursive();
	list2.print();

	cout << "***********************************************" << endl;

	int array[] = { -5, -3, -1, 0, 2, 3, 6 };
	cout << binarySearch(array, 7) << endl;

	cout << "***********************************************" << endl;

	Queue<int> q;
	Stack<int> s;
	q.EnQueue(1);
	q.EnQueue(2);
	q.EnQueue(3);
	q.EnQueue(4);
	q.Print();
	transerFromQueueToStack(q, s);
	s.Print();

	cout << "***********************************************" << endl;

	cout << computeNumberOfCombination(6, 2) << endl;
	cout << computeNumberOfCombination(6, 1) << endl;
	cout << computeNumberOfCombination(6, 4) << endl;

	cout << "***********************************************" << endl;

	int array1[] = { 1, 2, 3, 4, 5, 6 };
	reverseArrayResursive(array1, 6);
	for (int i = 0; i < 6; ++i)
	{
		cout << array1[i] << "\t";
	}
	cout << endl;

	cout << "***********************************************" << endl;

	int array2[] = { -23, -16, 0, 4, 6, 32, 77 };
	cout << binarySearchResursive(array2, 7, 77) << endl;
	cout << binarySearchResursive(array2, 7, -23) << endl;
	cout << binarySearchResursive(array2, 7, 32) << endl;
	cout << binarySearchResursive(array2, 7, 15) << endl;

	cout << "***********************************************" << endl;

	int array3[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	shiftArrayCounterclockwise(array3, 12, 4);
	for (int i = 0; i < 12; ++i)
	{
		cout << array3[i] << "\t";
	}
	cout << endl;
}


int findMissingElement(SList<int>& list, int n)
{
	SList<int>::Iterator i = list.Begin();
	int sum = 0;
	int totalSum = n*(n + 1) / 2;
	while (i != list.End())
	{
		sum += *i++;
	}
	return totalSum - sum;
}

int binarySearch(int a[], int size)
{
	int low, high, mid;
	low = 0;
	high = size - 1;

	while (low <= high)
	{
		mid = (low + high) / 2;
		if (a[mid] > mid)
		{
			high = mid - 1;
		}
		else if (a[mid] < mid)
		{
			low = mid + 1;
		}
		else
		{
			return mid;
		}
	}
	return -1;
}

int binarySearchResursive(int a[], int n, int value)
{
	return binarySearchResursiveImp(a, 0, n - 1, value);
}

int binarySearchResursiveImp(int a[], int low, int high, int value)
{
	if (low > high)
		return -1;

	int mid = (low + high) / 2;
	if (a[mid] == value)
		return mid;
	else if (a[mid] > value)
		return binarySearchResursiveImp(a, low, mid - 1, value);
	else
		return binarySearchResursiveImp(a, mid + 1, high, value);

}

void transerFromQueueToStack(Queue<int> &q, Stack<int> &s)
{
	while (!q.IsEmpty())
	{
		s.Push(q.Front());
		q.DeQueue();
	}

	while (!s.IsEmpty())
	{
		q.EnQueue(s.Top());
		s.Pop();
	}

	while (!q.IsEmpty())
	{
		s.Push(q.Front());
		q.DeQueue();
	}
}

int factorial(int n)
{
	assert(n >= 0);
	if (n == 0 || n == 1)
		return 1;
	else
		return n * factorial(n - 1);
}

int computeNumberOfCombination(int n, int m)
{
	assert(n >= m);
	return factorial(n) / (factorial(m) * factorial(n - m));
}

void reverseArrayResursive(int a[], int length)
{
	reverseArrayResursiveImp(a, 0, length - 1);
}

void reverseArrayResursiveImp(int a[], int i, int j)
{
	if (i >= j)
		return;
	else
	{
		int temp = a[i];
		a[i] = a[j];
		a[j] = temp;
		reverseArrayResursiveImp(a, ++i, --j);
	}
}

void shiftArrayCounterclockwise(int a[], int n, int k)
{
	assert(k >= 0 && n > 0);
	k = k % n;
	reverseArray(a, 0, k - 1);
	reverseArray(a, k, n - 1);
	reverseArray(a, 0, n - 1);
}

void reverseArray(int a[], int begin, int end)
{
	for (int i = begin, j = end; i < j; ++i, --j)
	{
		int temp = a[i];
		a[i] = a[j];
		a[j] = temp;
	}
}



