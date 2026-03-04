# ==函数==
## 1.函数基础
```c++
//
int main()
{

}
```
## 2.参数传递
```c++
int main()
{
    
}
```
## 3.返回类型和return
```c++
int main()
{
    
}
```
## 4.函数重载
```c++
int main()
{
    
}
```
## 5.特殊用途语言特性
```c++
int main()
{
    
}
```
## 6.函数匹配
```c++
int main()
{
    
}
```
## 7.函数指针
```c++
int main()
{
    
}
```
## 8.函数模板
### 1.模板的意义：对类型也可以进行参数化 int sum (int a,int b){return a+b;}
### 2.注意区分：函数模板 模板的实例化 模板函数 模板类型参数 模板非类型参数 模板的实参推演 模板的特例化等
```c++
/*
函数模板 
模板的实例化 
模板函数 
模板类型参数 
模板非类型参数
模板的实参推演
模板的特例化（专用化）
模板函数、模板的特例化、非模板函数的重载关系

*/
#include <iostream>
using namespace std;
//template<class T>
template<typename T>//定义一个模板参数列表
bool compare(T a, T b)//compare 现在是一个函数模板，通过该模板可以创建出具体的函数
{
	cout << "call template compare" << endl;
	return a > b;
}
int main()
{
	compare<int>(10, 20);//模板名<类型名>(参数1，参数2)；====>具体的函数
	
	return 0;
}
```