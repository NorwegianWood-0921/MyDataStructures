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
	//���캯�������ƹ��캯������������
	arrayList(int initialCapacity = 10, int initialMultiple = 2);
	arrayList(const arrayList<T>&);
	~arrayList() { delete[] element; }

	//ADT ����
	bool empty() const { return listSize == 0; }
	int size() const { return listSize; }
	T& get(int theIndex) const;
	int indexOf(const T& theElement) const;
	void erase(int theIndex);
	void insert(int theIndex, const T& theElement);
	void output(std::ostream& out = std::cout) const;

	//��������
	int capacity() const { return arrayLength; }  // �������鵱ǰ����
	void trimToSize();  // �ı������Сʹ�������Ա��С��ȣ������С��СΪ1��
	void setSize(int newSize);  // �������Ա�Ϊָ����С������С��ɾ������Ԫ�أ������������仯
	T& operator[](int theIndex);
	bool operator==(const arrayList<T>& other) const;
	bool operator!=(const arrayList<T>& other) const;

	//������iterator��Ϊ��arrayList��˫�������
	class iterator;
	iterator begin() { return iterator(element); }
	iterator end() { return iterator(element + listSize); }

protected:
	void checkIndex(int theIndex) const;  // ��������Ч�����׳��쳣

	T* element;  // �������Ա�Ԫ�ص�һά����
	int arrayLength;  // һά�����С
	int listSize;  // ���Ա�Ԫ�ظ���
	int multiple;  // ���������С�������С�ı�ı���
};

template<class T>
arrayList<T>::arrayList(int initialCapacity, int initialMultiple)
{//���캯��
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
{//���ƹ��캯��
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
class iterator  // ��arrayList��˫�������
{
public:
	// ��C++��typedef���ʵ��˫�������
	typedef bidirectional_iterator_tag iterator_category;  // ���������ͱ�ǩ����ʾ֧��˫�����
	typedef T value_type;  // ������ָ���Ԫ�����͡�
	typedef ptrdiff_t difference_type;  // ��ʾ����������֮��ľ��������
	typedef T* pointer;  // ��ʾ������ָ��Ԫ�����͵�ָ������
	typedef T& reference;  // ��ʾ����������Ԫ�����͵���������

	//���캯��
	iterator(T* thePosition = 0) { position = thePosition; }

	//�����ò�����
	T& operator*() const { return *position; }
	T* operator->() const { return &*position; }

	//������ֵ����
	iterator& operator++() { ++position; return *this; } // ǰ��
	iterator operator++(int)  // ���
	{
		iterator old = *this;
		++position;
		return old;
	}

	//������ֵ����
	iterator& operator--() { --position; return *this; } // ǰ��
	iterator operator--(int)  // ���
	{
		iterator old = *this;
		--position;
		return old;
	}

	//�����Ƿ����
	bool operator!=(const iterator& right) const { return position != right.position; }
	bool operator==(const iterator& right) const { return position == right.position; }

protected:
	T* position;
};

#endif