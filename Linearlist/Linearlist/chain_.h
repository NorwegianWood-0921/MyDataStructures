#ifndef chainNode_
#define chainNode_

#include <iostream>
#include <sstream>
#include "linearList_.h"
#include "myExceptions_.h"

template<class T>
struct chainNode  // ����ڵ�Ľṹ����
{
	//���ݳ�Ա
	T element;  // ������
	chainNode<T>* next;  // ָ����һ���ڵ㣨����

	//����
	chainNode() {}
	chainNode(const T& element) { this->element = element; }
	chainNode(const T& element, chainNode<T>* next) { this->element = element; this->next = next; }
};

template<class T>
class chain : public linearList<T>
{
public:
	chain(int initialCapacity = 10);
	chain(const chain<T>&);
	~chain();

	//ADT ����
	bool empty() const { return listSize == 0; }
	int size() const { return listSize; }
	T& get(int theIndex) const;
	int indexOf(const T& theElement) const;
	void erase(int theIndex);
	void insert(int theIndex, const T& theElement);
	void output(std::ostream& out = std::cout) const;

	//��������
	void binSort(int range);  // Ͱ����

protected:
	void checkIndex(int theIndex) const;
	chainNode<T>* firstNode;
	int listSize;
};

template<class T>
chain<T>::chain(int initialCapacity)
{//���캯��
	if (initialCapacity < 1)
	{
		std::ostringstream s;
		s << "Initial capacity = " << initialCapacity << "Must be > 0";
		throw illegalParameterValue(s.str());
	}
	firstNode = NULL;
	listSize = 0;
}

template<class T>
chain<T>::chain(const chain<T>& thelist)
{//���ƹ��캯��
	listSize = thelist.listSize;

	if (listSize == 0)
	{//����thelistΪ��
		firstNode = NULL;
		return;
	}

	//����thelistΪ�ǿ�
	chainNode<T>* sourceNode = thelist.firstNode;  // ����thelist�׽ڵ�
	firstNode = new chainNode<T>(sourceNode->element);  // ����thelist����Ԫ��  // ����chainNode�Ĺ��칹�캯���������� sourceNode->element ��Ϊ��������ʾ�� sourceNode �� element ֵ��ʼ���½ڵ�� element��
	sourceNode = sourceNode->next;  // ָ��thelist��һ���ڵ�
	chainNode<T>* targetNode = firstNode;  // ָ��ǰ����*this�����һ���ڵ�
	while (sourceNode != NULL)
	{//����ʣ��ڵ�
		targetNode->next = new chainNode<T>(sourceNode->element);
		targetNode = targetNode->next;
		sourceNode = sourceNode->next;
	}
	targetNode = NULL;  // �������
}

template<class T>
chain<T>::~chain()
{//��������
	while (firstNode != NULL)  // �ظ�ɾ���׽ڵ㣬ֱ������Ϊ��
	{//ɾ���׽ڵ�
		chainNode<T>* nextNode = firstNode->next;
		delete firstNode;
		firstNode = nextNode;  // ����һ���ڵ��Ϊ�׽ڵ㣻
	}
}

template<class T>
T& chain<T>::get(int theIndex) const
{
	checkIndex(theIndex);  // ����Ԫ���Ƿ�����Ƿ�

	//��������Ҫ�Ľڵ�
	chainNode<T>* currentNode = firstNode;
	for (int i = 0; i < theIndex; i++)
		currentNode = currentNode->next;
	return currentNode->element;
}

template<class T>
int chain<T>::indexOf(const T& theElement) const
{
	chainNode<T>* currentNode = firstNode;
	int index = 0;
	while (currentNode != NULL && currentNode->element != theElement) {
		currentNode = currentNode->next;  //������һ���ڵ�
		index++;
	}
	if (currentNode == NULL) return -1;
	else return index;
}

template<class T>
void chain<T>::erase(int theIndex)
{
	checkIndex(theIndex);

	chainNode<T>* deleteNode;
	// ��ɾ����Ԫ��ʱ
	if (theIndex == 0) {
		deleteNode = firstNode;
		firstNode = firstNode->next;
	}
	else {
		//��ɾ������Ԫ��ʱ
		chainNode<T>* currentNode = firstNode;
		for (int i = 0; i < theIndex - 1; i++)  // ָ���ɾ���ڵ��ǰ���ڵ�
			currentNode = currentNode->next;
		deleteNode = currentNode->next;
		currentNode->next = currentNode->next->next;
	}

	listSize--;
	delete deleteNode;
}

template<class T>
void chain<T>::insert(int theIndex, const T& theElement)
{
	if (theIndex < 0 || theIndex > listSize)
	{
		std::ostringstream s;
		s << "index = " << theIndex << " size = " << listSize;
		throw illegalIndex(s.str());
	}

	if (theIndex == 0) {//�ڱ�ͷ����
		firstNode = new chainNode<T>(theElement, firstNode);
	}
	else {//�ڷǱ�ͷ����
		chainNode<T>* currentNode = firstNode;
		for (int i = 0; i < theIndex - 1; i++)  // ָ��Ŀ��������Ӧ�ڵ��ǰ���ڵ�
			currentNode = currentNode->next;
		currentNode->next = new chainNode<T>(theElement, currentNode->next);
	}

	listSize++;	
}

template<class T>
void chain<T>::output(std::ostream& out) const
{//��������������
	for (chainNode<T>* currentNode = firstNode; currentNode != NULL; currentNode = currentNode->next)
		out << currentNode->element << " ";
}
template<class T>
ostream& operator<<(ostream& out, const chain<T>& x)
{
	x.output(out);
	return out;
}

template<class T>
void chain<T>::checkIndex(int theIndex) const
{
	if (theIndex < 0 || theIndex >= listSize)
	{
		std::ostringstream s;
		s << "index = " << theIndex << " size = " << listSize;
		throw illegalIndex(s.str());
	}
}

template<class T>
void chain<T>::binSort(int range)
{//Ͱ����
	//��������ʼ��Ͱ
	chainNode<T>** bottom, ** top;
	bottom = new chainNode<T>*[range + 1];
	top = new chainNode<T>*[range + 1];
	for (int i = 0; i <= range; i++)
		bottom[i] = NULL;

	//������Ľڵ���䵽Ͱ
	for (; firstNode != NULL; firstNode = firstNode->next)
	{
		int theBin = firstNode->element;  // Ԫ������ת��int
		if (bottom[theBin] == NULL)  // ͰΪ��
			bottom[theBin] = top[theBin] = firstNode;
		else {  // Ͱ�ǿ�
			top[theBin]->next = firstNode;
			top[theBin] = firstNode;
		}
	}

	//��Ͱ�еĽڵ��ռ�����������
	chainNode<T>* y = NULL;
	for(int theBin=0;theBin<=range;theBin++)
		if (bottom[theBin] != NULL)  // Ͱ�ǿ�
		{
			if (y == NULL)  // ��һ���ǿ�Ͱ
				firstNode = bottom[theBin];
			else
				y->next = bottom[theBin];
			y = top[theBin];
		}
	if (y != NULL)
		y->next = NULL;

	delete[] bottom;
	delete[] top;
}

#endif