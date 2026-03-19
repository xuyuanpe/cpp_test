# STL standard template library 标准模板库
## 1.学习内容简介
```C++
/*
一.标准容器
1.顺序容器:顺序表
vector
deque
list
2.容器适配器
stack
queue
priority——queue
3.关联容器
a 无序关联容器 链式哈希表 增删查O（1） 哈希冲突
unordered_set
unordered_multiset
unordered_map
unordered_multimap
b 有序关联容器 红黑树 增删查 O（log2n）
set
multiset
map
multimap
二.近容器
数组 string bitset(位容器)
三.迭代器
iterator const_iterator 正向迭代器
reverse_iterator const_reverse_iterator 反向迭代器
四.函数对象(类似c的函数指针) 
greater less
五.泛型算法
sort find find_if binary_search for_each
*/
```
## 2.vcetor
```C++
#include<iostream>
#include<vector>
using namespace std;
/*
* 1.vector 向量容器
* 2.底层数据结构，动态开辟的数组，每次已原来二倍大小进行扩容
* 增删改查 常用方法介绍
* vector<int>vec
* vec.push_back(20); 末尾添加元素 O(1) 导致扩容
* vec.insert(it,20); 在指定位置添加元素，it是迭代器指向的位置 O(n) 
* vec.pop_back();末尾删除元素 O(1)
* vec.erase(it);删除迭代器指向元素O(n)
* operator[]--->通过下标访问vec[5] O(1)
* iterator 迭代器遍历
* find /for each泛型算法
* 对容器进行连续插入或删除操作：insert erase，一定要更新迭代器--->空间变化引起迭代器失效
* size()--->返回有效个数
* empty（）-->判空
* reserve(20)---->vector预留空间
* resize()---->重置大小
* swap（）---->元素交换
* 
*/
int main()
{
	vector<int> vec;
	//vec.reserve(20);//先分配空间，避免重复扩容 只开辟空间，不会添加新的元素
	vec.resize(20);//
	cout << vec.empty() << endl;
	cout << vec.size() << endl;
	for (int i = 0; i < 20; ++i)
	{
		vec.push_back(rand() % 100 + 1);
	}
	cout << vec.empty() << endl;
	cout << vec.size() << endl;
#if 0
	//输出
	int size = vec.size();
	cout << "vec.size: " << size << endl;
	for (int i = 0; i < size; ++i)
	{
		cout << vec[i] << " " ;//通过下标访问
	}
	cout << endl;
	//通过迭代器访问
	
	/*auto it = vec.begin();
	for (; it != vec.end();++it)
	{
		cout << *it << " ";
	}*/
	cout << endl;
	//删除vector元素中所有的偶数
	auto it2 = vec.begin();
	while(it2 != vec.end())//删除一个元素后后面的元素会往前挪，所以下一次判断时迭代器也是在这个位置
	{
		if (*it2 % 2 == 0)
		{
			it2=vec.erase(it2);//迭代器更新
		}
		else
		{
			++it2;
		}
	}
	//每一个奇数元素前面加一个比他小1的偶数
	auto it3 = vec.begin();
	for (it3; it3 != vec.end();++it3)
	{
		if (*it3 % 2 !=0)
		{
			it3 = vec.insert(it3, *it3 - 1);
			++it3;
		}
	}
	auto it = vec.begin();
	while (it != vec.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
#endif
	return 0;
}
```
## 3.deque
## 4.list
## 5.容器适配器
## 6.无序关联容器
## 7.有序关联容器
## 8.迭代器iterator
## 9.函数对象
## 10.泛型算法和绑定器