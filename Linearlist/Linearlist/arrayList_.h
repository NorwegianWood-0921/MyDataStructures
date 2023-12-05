//arrayList class for array representation of linear list

#ifndef arrayList_
#define arrayList_

#include <iostream>
#include <sstream>
#include "myExceptions_.h"
#include "linearList_.h"
#include "changeLength_.h"

template<class T>
class arrayList : public linearList<T>
{
public:
	//构造函数，复制构造函数，析构函数
	arrayList(int initialCapacity = 10, int initialMultiple = 2);
	arrayList(const arrayList<T>&);
	~arrayList() { delete[] element; }

	//ADT 方法
	bool empty() const { return listSize == 0; }
	int size() const { return listSize; }
	T& get(int theIndex) const;
	int indexOf(const T& theElement) const;
	void erase(int theIndex);
	void insert(int theIndex, const T& theElement);
	void output(std::ostream& out = std::cout) const;

	//其它方法
	int capacity() const { return arrayLength; }  // 给出数组当前长度
	void trimToSize();  // 改变数组大小使其与线性表大小相等（数组大小最小为1）
	void setSize(int newSize);  // 设置线性表为指定大小，若缩小则删减超出元素，若扩大则不做变化
	T& operator[](int theIndex);
	bool operator==(const arrayList<T>& other) const;
	bool operator!=(const arrayList<T>& other) const;

	//定义类iterator作为类arrayList的双向迭代器
	class iterator;
	iterator begin() { return iterator(element); }
	iterator end() { return iterator(element + listSize); }

protected:
	void checkIndex(int theIndex) const;  // 若索引无效，则抛出异常

	T* element;  // 储存线性表元素的一维数组
	int arrayLength;  // 一维数组大小
	int listSize;  // 线性表元素个数
	int multiple;  // 超出数组大小是数组大小改变的倍数
};

template<class T>
arrayList<T>::arrayList(int initialCapacity, int initialMultiple)
{//构造函数
	if (initialCapacity < 1)
	{
		std::ostringstream s;
		s << "Initial capacity = " << initialCapacity << "Must be > 0";
		throw illegalParameterValue(s.str());
	}
	if (initialMultiple <= 1)
	{
		std::ostringstream s;
		s << "The multiple of the array size change = " << initialMultiple << "Must be > 1";
		throw illegalParameterValue(s.str());
	}
	arrayLength = initialCapacity;
	element = new T[arrayLength];
	listSize = 0;
	multiple = initialMultiple;
}

template<class T>
arrayList<T>::arrayList(const arrayList<T>& theList)
{//复制构造函数
	arrayLength = theList.arrayLength;
	listSize = theList.listSize;
	element = new T[arrayLength];
	std::copy(theList.element, theList.element + listSize, element);
}

template<class T>
void arrayList<T>::checkIndex(int theIndex) const
{
	if (theIndex < 0 || theIndex >= listSize)
	{
		std::ostringstream s;
		s << "index = " << theIndex << " size = " << listSize;
		throw illegalIndex(s.str());
	}
}

template<class T>
T& arrayList<T>::get(int theIndex) const
{
	checkIndex(theIndex);
	return element[theIndex];
}
template<class T>
T& arrayList<T>::operator[](int theIndex)
{
	return get(theIndex);
}

template<class T>
int arrayList<T>::indexOf(const T& theElement) const
{
	int theIndex = (int)(find(element, element + listSize, theElement) - element);
	if (theIndex == listSize)
		return -1;
	else return theIndex;
}

template<class T>
void arrayList<T>::erase(int theIndex)
{
	checkIndex(theIndex);
	copy(element + theIndex + 1, element + listSize, element + theIndex);
	element[--listSize].~T();
}

template<class T>
void arrayList<T>::insert(int theIndex, const T& theElement)
{
	if (theIndex < 0 || theIndex > listSize)
	{
		std::ostringstream s;
		s << "index = " << theIndex << " size = " << listSize;
		throw illegalIndex(s.str());
	}

	if (listSize == arrayLength)
	{
		changeLength1D(element, arrayLength, multiple * arrayLength);
		arrayLength *= multiple;
	}

	copy_backward(element + theIndex, element + listSize, element + listSize + 1);
	element[theIndex] = theElement;
	listSize++;
}

template<class T>
void arrayList<T>::output(std::ostream& out) const
{
	copy(element, element + listSize, std::ostream_iterator<T>(cout, " "));
}
template<class T>
std::ostream& operator<<(std::ostream& out, const arrayList<T>& x)
{
	x.output(out);
	return out;
}

template<class T>
void arrayList<T>::trimToSize()
{
	int newLength = max(listSize, 1);
	changeLength1D(element, arrayLength, newLength);
	arrayLength = newLength;
}

template<class T>
void arrayList<T>::setSize(int newSize)
{
	if (newSize < listSize) {
		int number = listSize - newSize;
		for (int i = 0; i < number; i++) erase(newSize);
	}
}

template<class T>
bool arrayList<T>::operator==(const arrayList<T>& other) const
{
	if (listSize == other.listSize) {
		for (int i = 0; i < listSize; i++)
			if (element[i] != other.element[i]) return 0;
		return 1;
	}
	return 0;
}
template<class T>
bool arrayList<T>::operator!=(const arrayList<T>& other) const
{
	return !(*this == other);
}

template<class T>
class iterator  // 类arrayList的双向迭代器
{
public:
	// 用C++的typedef语句实现双向迭代器
	typedef bidirectional_iterator_tag iterator_category;  // 迭代器类型标签，表示支持双向遍历
	typedef T value_type;  // 迭代器指向的元素类型。
	typedef ptrdiff_t difference_type;  // 表示两个迭代器之间的距离的类型
	typedef T* pointer;  // 表示迭代器指向元素类型的指针类型
	typedef T& reference;  // 表示迭代器引用元素类型的引用类型

	//构造函数
	iterator(T* thePosition = 0) { position = thePosition; }

	//解引用操作符
	T& operator*() const { return *position; }
	T* operator->() const { return &*position; }

	//迭代器值增加
	iterator& operator++() { ++position; return *this; } // 前加
	iterator operator++(int)  // 后加
	{
		iterator old = *this;
		++position;
		return old;
	}

	//迭代器值减少
	iterator& operator--() { --position; return *this; } // 前减
	iterator operator--(int)  // 后减
	{
		iterator old = *this;
		--position;
		return old;
	}

	//测试是否相等
	bool operator!=(const iterator& right) const { return position != right.position; }
	bool operator==(const iterator& right) const { return position == right.position; }

protected:
	T* position;
};

#endif