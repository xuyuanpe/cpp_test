#include<iostream>
#include<vector>
using namespace std;
/*
* 1.正向迭代器(iterator)不仅可以读取元素，也可以修改 删除
* 2.常量正向迭代器（const_iterator）: 只读
* 3.reverse_iterator :反向迭代器
* 4.常量反向迭代器 const reverse_iterator
*/
int main(int* argc, char* argv[])
{
	vector<int> vec;
	for (int i = 0; i < 20; ++i)
	{
		vec.push_back(rand() % 100);
	}
	vector<int>::iterator it = vec.begin();//auto it =vec.begin();
	for (; it != vec.end(); ++it)
	{
		cout << *it << " ";
		if (*it % 2 == 0)
		{
			*it = 0;
		}

	}
	cout << endl;
	cout << "----------------------------------------------------------" << endl;
	for (int v : vec)
	{
		cout << v << " ";
	}
	cout << endl;
	cout << "----------------------------------------------------------" << endl;
	vector<int>::reverse_iterator rit = vec.rbegin();//rbegin()---最后一个元素位置    rend（）第一个元素的前驱
	for (; rit != vec.rend(); ++rit)
	{
		cout << *rit << " ";
	}
	cout << endl;
	cout << "----------------------------------------------------------" << endl;
	return 0;
}
