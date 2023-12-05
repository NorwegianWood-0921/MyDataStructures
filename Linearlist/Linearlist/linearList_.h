//the ADT(abstract data type) of linear list

#ifndef linearList_
#define linearList_

#include <iostream>

template<class T>
class linearList  // ���Ա�ĳ�����
{
public:
	virtual ~linearList() {};  // ��������
	virtual bool empty() const = 0;  // ��Ϊ���򷵻�true
	virtual T& get(int theIndex) const = 0;  // �������Ա��ж�Ӧ������Ԫ��
	virtual int indexOf(const T& theElement) const = 0;  // �������Ա��е�һ�γ��ָ�Ԫ�ص�����������Ԫ�ز����ڣ��򷵻�-1
	virtual void erase(int theIndex) = 0;  // ɾ����������Ӧ��Ԫ��
	virtual void insert(int theIndex, const T& theElement) = 0;  // ���뵽������Ӧλ��
	virtual void output(std::ostream& out) const = 0;  // �����������Ԫ��
};

#endif