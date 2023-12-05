#ifndef changeLength_
#define changeLength_
#include "myExceptions_.h"

template<class T>
void changeLength1D(T*& a, int oldLength, int newLength)
{//�ı����鳤��
	if (newLength < 0)
		throw illegalParameterValue("new length must be >= 0");

	T* temp = new T[newLength];
	int number = min(oldLength, newLength);
	std::copy(a, a + number, temp);
	delete[] a;
	a = temp;
}

template<class T>
void changeLength2D(T**& a, int oldRows, int oldCols,int newRows, int newCols)
{//�ı��ά���鳤��
	if (newRows < 0 || newCols < 0)
		throw illegalParameterValue("new rows and new columns must be >= 0");

	//����������
	T** temp = new T * [newRows];
	for (int i = 0; i < newRows; i++) {
		temp[i] = new T[newCols];
	}

	//����ԭ������������
	int numRows = min(oldRows, newRows);
	int numCols = min(oldCols, newCols);
	for (int i = 0; i < numRows; i++) {
		std::copy(a[i], a[i] + numCols, temp[i]);
	}

	//�ͷ�ԭ����ռ�
	for (int i = 0; i < oldRows; ++i) {
		delete[] a[i];
	}
	delete[] a;

	// ����ָ��
	a = temp;
}

#endif
