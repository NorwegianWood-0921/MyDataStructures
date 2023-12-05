//the ADT(abstract data type) of linear list

#ifndef linearList_
#define linearList_

#include <iostream>

template<class T>
class linearList  // 线性表的抽象类
{
public:
	virtual ~linearList() {};  // 析构函数
	virtual bool empty() const = 0;  // 若为空则返回true
	virtual T& get(int theIndex) const = 0;  // 返回线性表中对应索引的元素
	virtual int indexOf(const T& theElement) const = 0;  // 返回线性表中第一次出现该元素的索引，若该元素不存在，则返回-1
	virtual void erase(int theIndex) = 0;  // 删除该索引对应的元素
	virtual void insert(int theIndex, const T& theElement) = 0;  // 插入到索引对应位置
	virtual void output(std::ostream& out) const = 0;  // 从左到右输出表元素
};

#endif