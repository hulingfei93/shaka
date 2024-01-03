#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <iostream>
#include "SList.h"

template <class T>
class Queue
{
private:
    SList<T> container;
public:
    const T& Front()
    {
        return container.ShowFirst();
    }

    void DeQueue()
    {
        container.RemoveFirst();
    }

    void EnQueue(const T& newItem)
    {
        container.AddLast(newItem);
    }

    bool IsEmpty()
    {
        return container.IsEmpty();
    }

    void Print()
    {
        SList<T>::Iterator i = container.Begin();
        while (i != container.End())
        {
            std::cout << *i++ << "\t";
        }
        std::cout << std::endl;
    }

    void Print_v2()
    {
        SList<T>::Iterator i = container.Begin();
        while (i != container.End())
        {
            std::cout << *i++;
        }
        std::cout << std::endl;
    }
};


#endif