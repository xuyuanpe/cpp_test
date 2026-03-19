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
```C++
#include<iostream>
#include<deque>
using namespace std;
/*
* deque--->双端队列容器--->底层数据结构是二维数组
* deque<int>deq;
* deq.push_back(20);后
* deq.push_front(20);前
* deq.insert(it,20);
* deq.pop_back();末尾删除
* deq.pop_front();头部删除
* deq.erase(it);迭代器指向位置删除
* 连续的insert/erase需要考虑迭代器失效问题
* 
* 
*/
int main()
{

}
```
## 4.list
```C++
#include<iostream>
#include<list>

using namespace std;
/*list 链表容器  底层数据结构 双向循环链表 pre data last
* list<int>mylist;
* mylist.push_back(20);后
* mylist.push_front(20);前
* mylist.insert(it,20); //插入之前先进行query查询操作
* mylist.pop_back();末尾删除
* mylist.pop_front();头部删除
* mylist.erase(it);迭代器指向位置删除
* 连续的insert/erase需要考虑迭代器失效问题
*/
int main()
{
	return 0;
}
```
### 顺序容器总结
```C++
#include<iostream>
#include<deque>
#include<list>
#include<vector>
using namespace std;
/*纵向：各个容器的掌握程度
* 横向：各个容器之间的区别
* vector list deque
* 1.vector -->底层是动态开辟的数组 内存是连续的 2倍扩容 
* 默认定义的vector<int> vec;-->此时还没有开辟空间 push_back\insert 0-1-2-4-8-扩容 使用reserve预留空间
* 2.deque -->底层是动态开辟的二维数组 第二维是固定长度的数组空间 扩容是一维以二倍变化 deque 内存并不是连续的 第二维是单独new出来的
* 也可以说是每一个二维是连续的
* 3.vector和deque之间的区别：
* a.底层数据结构不一样
* b.插入删除元素复杂度
* c.内存使用效率
* d.插入和删除元素的效率 前中后 是否移动元素 内存完全连续的挪动比较方变
* 4.list：双向循环链表
* 
* 
*/
int main()
{
	return 0;
} 
```
## 5.容器适配器
```C++
#include<iostream>
#include<deque>
#include<vector>
#include<list>
#include<stack>
#include<queue>
using namespace std;
/*
* 1.适配器底层没有自己的数据结构，它是另外一个容器的封装，它的方法都是依赖底层容器进行实现
* 2.没有实现自己的迭代器
* 
template<typename T,typename Container =deque<T>>
class stack
{
public:
	void push(const T& val) { con.push_back(val); }
	void pop() { con.pop_back(); }
	T top()const { return con.back; }
private:
	Container con;
};
*/
/*
* 初始内存使用效率
* queue队尾入队头出 使用deque效率更高
* deque更适合存储大量数据
* 大根堆
*/
int main()
{
	//stack push pop top empty size	
	//---->底层是deque
	stack<int> s1;
	for (int i = 0; i < 20; ++i)
	{
		s1.push(rand() % 100 + 1);
	}
	cout << s1.size() << endl;
	while (!s1.empty())
	{
		cout << s1.top() << " ";
		s1.pop();
	}
	cout << endl;
	cout << "-------------------------------------------------" << endl;
	//queue push pop front back empty size
	//---->底层是deque
	queue<int>que;
	for (int i = 0; i < 20; ++i)
	{
		que.push(rand() % 100 + 1);
	}
	cout << que.size() << endl;
	while (!que.empty())
	{
		cout << que.front() << " ";
		que.pop();
	}
	cout << endl;
	cout << "-------------------------------------------------" << endl;
	//priority_queue push pop top empty size
	//--->底层是vector
	priority_queue<int> pque;
	for (int i = 0; i < 20; ++i)
	{
		pque.push(rand() % 100 + 1);
	}
	cout << pque.size() << endl;
	while (!pque.empty())
	{
		cout << pque.top() << " ";//优先级大的先出队
		pque.pop();
	}
	cout << endl;



	return 0;
}
```
## 6.无序关联容器
```C++
#include<iostream>
#include<deque>
#include<list>
#include<vector>
#include<stack>
#include<queue>
#include<set>
#include<map>
#include<unordered_set>
#include<unordered_map>
#include<string>
using namespace std;

/*
* 关联容器
* 1.无序关联容器--->链式哈希表 增删查O(1)
* set:集合 key    map：映射表 {key;value}---键值对
* unordered_set 单重集合 不会存储k值重复的元素
* unordered_multiset 多重集合
* unordered_map 单重映射表
* unordered_multimap 多重映射表
* 2.有序关联容器--->红黑树 增删查O(log2n) 
* set
* multiset
* map
* multimap
* 3.insert()  遍历使用迭代器 find方法 erase(key)/rease(it)
* 
*/

int main()
{
#if 0
	//unordered_set
	unordered_set<int> set1; 
	//unordered_multiset<int> set1;//多重集合可以存储重复元素
	for (int i = 0; i < 50; ++i)
	{
		set1.insert(rand() % 20 + 1);
	}
	cout << set1.size() << endl;//size=18，说明50个里面有重复元素，单重集合不存储重复元素
	cout << set1.count(15) << endl;//返回key=15 的元素个数
	
	auto it = set1.begin();
	for (; it != set1.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;
	//set1.erase(20);//按key值删除元素
	for (it = set1.begin(); it != set1.end();)
	{
		if (*it == 19)
		{
			it = set1.erase(it);//更新迭代器
		}
		else
		{
			++it;
		}
	}
	it = set1.find(20);
	if (it != set1.end())//找不到会返回end（）
	{
		set1.erase(it);
	}
	for (auto v : set1)
	{
		cout << v << " ";
	}
	cout << endl;

#endif
#if 0
	//unordered_map:
	/*
	* class pair
	* {
	* public：
	* private：
	* first; =>key
	* second;=>value
	* }
	*/
	unordered_map<int, string> map1;
	//unordered_multimap<int, string> map1;
	map1.insert(make_pair(100, "a"));//键值对打包
	map1.insert({ 1010,"b" });//C++11标准
	map1.insert({ 1020,"c" });
	map1.insert({ 1000,"d" });//单重映射不允许键重复
	map1[3000] = "e";
	//cout << map1.size() << endl;
	//cout << map1[1000] << endl;
	//cout << map1[100] << endl;//map -->operator[]重载（key）return value 如果key不存在，会插入一对数据[key,string()]
	//map1.erase(100);//删除
	//cout << map1.size() << endl;
	auto it = map1.find(100);
	if (it != map1.end())
	{
		//it->pair对象
		cout << "key: " << it->first << endl;
		cout << "value: " << it->second << endl;
	}

#endif
#if 0
	//处理海量数据
	const int ARR_LEN = 100;
	int arr[ARR_LEN] = {0};
	for(int i = 0; i < ARR_LEN; ++i)
	{
		arr[i] = rand() % 20 + 1;
	}
	//统级哪些数字重复了，以及重复次数
	unordered_map<int, int> map1;//
	for (int k : arr)
	{
		/*auto it = map1.find(k);
		if (it == map1.end())
		{
			map1.insert({k,1});
		}
		else
		{
			it->second++;
		}*/
		map1[k]++;
	}
	for (const pair<int, int> &p : map1) //只遍历，不修改
	{
		/*if (p.second > 1)
		{
			cout << "key: " << p.first << " " << "value: " << p.second << endl;
		}*/
		cout << "key: " << p.first << " " << "value: " << p.second << endl;
	}
	cout << "================================================================" << endl;
	auto it2 = map1.begin();
	for (; it2 != map1.end(); ++it2)
	{
		cout << "key: " << it2->first << " " << "value: " << it2->second << endl;
	}
#endif
	//去重 去重打印  set map ---set单重集合
	const int ARR_LEN = 100;
	int arr[ARR_LEN] = { 0 };
	for (int i = 0; i < ARR_LEN; ++i)
	{
		arr[i] = rand() % 20 + 1;
	}
	unordered_set<int> set1;
	for (int v : arr)
	{
		set1.insert(v);
	}
	for (int i : set1)
	{
		cout << i << " ";
	}
	cout << endl;



	return 0;
}

```
## 7.有序关联容器
## 8.迭代器iterator
## 9.函数对象
## 10.泛型算法和绑定器