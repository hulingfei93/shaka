#ifndef _STACK_H_
#define _STACK_H_

#include <iostream>
#include "SList.h"

template <class T>
class Stack
{
private:
    SList<T> container;
public:
    const T& Top()
    {
        return container.ShowFirst();
    }

    void Pop()
    {
        container.RemoveFirst();
    }

    void Push(const T& newItem)
    {
        container.AddFirst(newItem);
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
};


#endif