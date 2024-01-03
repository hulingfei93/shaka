#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <assert.h>
template <class T>
class Vector {
public:
	Vector(int capacity = 10); // default constructor
	Vector(const Vector<T> &); // copy constructor
	const Vector<T>& operator=(const Vector<T> &); // operator=
	~Vector(); // destructor
	bool IsEmpty() const;
	int NumberOfItems() const;
	void AddLast(const T&);
	void RemoveLast();
	const T& ShowLast() const;
	T& operator[](int);
	const T& operator[](int) const;
	void Resize(int);
	void RemoveAll();
private:
	T *array;
	int number_of_items;
	int size;
}; // end of class Vector declaration
#endif