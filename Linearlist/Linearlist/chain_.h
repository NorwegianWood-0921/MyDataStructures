#ifndef chainNode_
#define chainNode_

#include <iostream>
#include <sstream>
#include "linearList_.h"
#include "myExceptions_.h"

template<class T>
struct chainNode  // 链表节点的结构定义
{
	//数据成员
	T element;  // 数据域
	chainNode<T>* next;  // 指向下一个节点（链域）

	//方法
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

	//ADT 方法
	bool empty() const { return listSize == 0; }
	int size() const { return listSize; }
	T& get(int theIndex) const;
	int indexOf(const T& theElement) const;
	void erase(int theIndex);
	void insert(int theIndex, const T& theElement);
	void output(std::ostream& out = std::cout) const;

	//其它方法
	void binSort(int range);  // 桶排序

protected:
	void checkIndex(int theIndex) const;
	chainNode<T>* firstNode;
	int listSize;
};

template<class T>
chain<T>::chain(int initialCapacity)
{//构造函数
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
{//复制构造函数
	listSize = thelist.listSize;

	if (listSize == 0)
	{//链表thelist为空
		firstNode = NULL;
		return;
	}

	//链表thelist为非空
	chainNode<T>* sourceNode = thelist.firstNode;  // 复制thelist首节点
	firstNode = new chainNode<T>(sourceNode->element);  // 复制thelist的首元素  // 调用chainNode的构造构造函数，传递了 sourceNode->element 作为参数，表示用 sourceNode 的 element 值初始化新节点的 element。
	sourceNode = sourceNode->next;  // 指向thelist下一个节点
	chainNode<T>* targetNode = firstNode;  // 指向当前链表*this的最后一个节点
	while (sourceNode != NULL)
	{//复制剩余节点
		targetNode->next = new chainNode<T>(sourceNode->element);
		targetNode = targetNode->next;
		sourceNode = sourceNode->next;
	}
	targetNode = NULL;  // 链表结束
}

template<class T>
chain<T>::~chain()
{//析构函数
	while (firstNode != NULL)  // 重复删除首节点，直至链表为空
	{//删除首节点
		chainNode<T>* nextNode = firstNode->next;
		delete firstNode;
		firstNode = nextNode;  // 将下一个节点变为首节点；
	}
}

template<class T>
T& chain<T>::get(int theIndex) const
{
	checkIndex(theIndex);  // 检测该元素是否存在是否

	//移向所需要的节点
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
		currentNode = currentNode->next;  //移向下一个节点
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
	// 当删除首元素时
	if (theIndex == 0) {
		deleteNode = firstNode;
		firstNode = firstNode->next;
	}
	else {
		//当删除非首元素时
		chainNode<T>* currentNode = firstNode;
		for (int i = 0; i < theIndex - 1; i++)  // 指向待删除节点的前驱节点
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

	if (theIndex == 0) {//在表头插入
		firstNode = new chainNode<T>(theElement, firstNode);
	}
	else {//在非表头插入
		chainNode<T>* currentNode = firstNode;
		for (int i = 0; i < theIndex - 1; i++)  // 指向目标索引对应节点的前驱节点
			currentNode = currentNode->next;
		currentNode->next = new chainNode<T>(theElement, currentNode->next);
	}

	listSize++;	
}

template<class T>
void chain<T>::output(std::ostream& out) const
{//将链表放入输出流
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
{//桶排序
	//创建并初始化桶
	chainNode<T>** bottom, ** top;
	bottom = new chainNode<T>*[range + 1];
	top = new chainNode<T>*[range + 1];
	for (int i = 0; i <= range; i++)
		bottom[i] = NULL;

	//把链表的节点分配到桶
	for (; firstNode != NULL; firstNode = firstNode->next)
	{
		int theBin = firstNode->element;  // 元素类型转到int
		if (bottom[theBin] == NULL)  // 桶为空
			bottom[theBin] = top[theBin] = firstNode;
		else {  // 桶非空
			top[theBin]->next = firstNode;
			top[theBin] = firstNode;
		}
	}

	//将桶中的节点收集到有序链表
	chainNode<T>* y = NULL;
	for(int theBin=0;theBin<=range;theBin++)
		if (bottom[theBin] != NULL)  // 桶非空
		{
			if (y == NULL)  // 第一个非空桶
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