#ifndef _SLIST_TEST_H_
#define _SLIST_TEST_H_

#include <iostream>
#include "SList.h"

using namespace std;

void printList(SList<int> &);

void SlistTest() 
{
	SList<int> list1;
	list1.AddFirst(5);
	list1.AddFirst(4);
	list1.AddFirst(3);
	list1.AddFirst(2);
	list1.AddFirst(1);
	printList(list1);

	SList<int>::Iterator i = list1.Begin();
	while (i != list1.End())
	{
		cout << *i++ << "\t";
	}
	cout << endl;

	i = list1.Begin();
	*i = 0;
	cout << list1.ShowFirst() << endl;

	list1.AddLast(6);
	list1.AddLast(7);
	list1.AddLast(8);
	list1.AddLast(9);
	list1.AddLast(10);
	printList(list1);

	i = list1.Begin();
	++i;
	list1.AddAfter(i, -1);
	list1.AddAfter(i, -2);
	list1.AddAfter(i, -3);
	list1.AddAfter(i, -4);
	list1.AddAfter(i, -5);
	printList(list1);

	list1.RemoveFirst();
	printList(list1);

	i = list1.Begin();
	list1.RemoveAfter(i);
	list1.RemoveAfter(i);
	printList(list1);

	SList<int> list2(list1);
	SList<int> list3 = list2;
	*list2.Begin() = 999;
	list1.SetToEmpty();
	printList(list1);
	printList(list2);
	printList(list3);
}

void printList(SList<int> &list)
{
	SList<int>::Iterator i;
	for (i = list.Begin(); i != list.End(); ++i)
	{
		cout << *i << "\t";
	}
	cout << endl;
}

#endif