#include "Vector.h"

template <class T>
Vector<T>::Vector(int capacity)
{
	assert(capacity > 0);
	array = new T[capacity];
	size = capacity;
	number_of_items = 0;
}

template <class T>
Vector<T>::Vector(const Vector<T> &ob)
{
	array = new T[ob.size];
	for (int i= 0; i < ob.number_of_items; i++)
		array[i] = ob.array[i];
	size = ob.size;
	number_of_items = ob.number_of_items;
}

template <class T>
const Vector<T>& Vector<T>::operator=(const Vector<T> &ob) 
{
	if (this != &ob) {
		delete[] array;
		array = new T[ob.size];
		for (int i = 0; i < ob.number_of_items; i++)
			array[i] = ob.array[i];
		size = ob.size;
		number_of_items = ob.number_of_items;
	}
	return *this;
}

template <class T> 
Vector<T>::~Vector() 
{
	delete[] array;
}

template <class T> 
bool Vector<T>::IsEmpty() const 
{
	return (number_of_items == 0);
}

template <class T> 
int Vector<T>::NumberOfItems() const 
{
	return number_of_items;
}

template <class T> 
void Vector<T>::AddLast(const T &newItem) 
{
	if (number_of_items == size)
		Resize(2 * size);
	array[number_of_items++] = newItem;
}

template <class T> 
void Vector<T>::RemoveLast() 
{
	assert(number_of_items > 0);
	--number_of_items;
}

template <class T> 
const T& Vector<T>::ShowLast() const 
{
	assert(number_of_items > 0);
	return array[number_of_items - 1];
}

template <class T> 
T& Vector<T>::operator[](int index) 
{
	assert(index < number_of_items && index >= 0);
	return array[index];
}

template <class T> 
const T& Vector<T>::operator[](int index) const 
{
	assert(index < number_of_items && index >= 0);
	return array[index];
}

template <class T> 
void Vector<T>::Resize(int capacity) 
{
	T* newArray = new T[capacity];
	int saveNumberOfItems = number_of_items;
	if (number_of_items > capacity)
		saveNumberOfItems = capacity;
	for (int i = 0; i < saveNumberOfItems; i++)
		newArray[i] = array[i];
	delete[] array;
	array = newArray;
	number_of_items = saveNumberOfItems;
	size = capacity;
}

template <class T> 
void Vector<T>::RemoveAll() 
{
	while (!IsEmpty())
		RemoveLast();
}
// End of vector class functions implementation