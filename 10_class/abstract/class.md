# ==class==
## 1. new和delete\区别于malloc和free
```c++
//1.malloc和free都是库函数，new和delete是运算符
int main()
{
    int *p =(int*)malloc(sizeof(int));//整型
    if(p==nullptr)
    {
        return -1;
    }
    *p =20;
    free(p);
    int *p1 =(int*)malloc(sizeof(int)*20);//数组类型
    free(p1);
    try
    {
        int *q =new int(20);//new可以在开辟内存的同时进行初始化操作、new失败会抛出bad_alloc类型的异常
    }
    catch(const bad_alloc *e)
    {

    }
    delete q;
    int *q1 =new int[20];//开辟，但是不初始化
    int *q2 =new int[20]();//开辟，并且初始化为0；
    delete []q1;//删除数组，而不是删除单个元素
    delete []q2;
    int *q3 =new (nothrow)int；//不抛出异常
    const int *q4 =new const int(40);//开辟常量内存

}
```
## 2. 定位new
```c++
int *p =new int(40);//在堆上不确定位置开辟空间
int data =0;
int *p =new(&data)int(40);//在指定位置开辟空间

```
## 3.类和对象：（对象是一个逻辑上的概念）
### 1.抽象和类
```c++
#include <iostream>
using namespace std;
const int NAME_LENGTH = 20;
class cgood
{
private://私有的成员变量
	char _name[NAME_LENGTH];
	double _price;
	int _amount;

public://共有的成员函数
	void init(const char* name, double price, int amount);//常量字符串不允许用普通指针来接收
	void show();
	void setName(char* name) { strcpy_s(_name, name); }//类体内实现的函数，自动处理成内联函数
	void setPrice(double price) { _price = price; }
	void setAmount(int amount) { _amount = amount; }
	const char* getName() { return _name; }
	double getPrice() { return _price; }
	int getAmount() { return _amount; }

};
//类外定义：想要处理成内联函数需要加上关键字inline
void cgood::init(const char* name, double price, int amount)//方法前面要加上类的作用域（返回值后面，函数名前面）
{
	strcpy_s(_name, name);//strcpy不安全
	_price = price;
	_amount = amount;

}
void cgood::show()
{
	cout << "name:" << _name << endl;
	cout << "price:" << _price << endl;
	cout << "amount:" << _amount << endl;
}
int main()
{
	//如何计算类的内存大小----类的大小只和成员变量有关，和成员方法无关
	//内存计算先找占用最长字节的变量，然后根据编译器的对齐方式排列，最后计算出来

	//一个类可以定义无数个对象，每个对象都有自己的成员变量，但是他们共享一套成员方法
	//	
	cgood good;//实例化一个对象
	good.init("面包", 100.5, 100);
	good.show();
	return 0;
}
```
### 2.构造函数和析构函数
```c++
//1.内存的初始化和释放在顺序栈中时手动操作的，很有可能一不注意就漏掉了，所以我们引入了构造函数和析构函数
//2.函数的名字和类名相同，没有返回值
//3.先构造的后析构，后构造的先析构---因为在栈上
//4.析构函数不带参数，所以析构函数只能有一个，构造函数可以重载（重载和返回值没有关系，c++的重载只和函数名和参数列表有关）
//5.析构函数可以自己调用，调用完对象也就不存在了，堆内存已经释放了，这个时候调用方法在语法上没有问题，但是已经是堆内存的非法访问了
//6.全局对象在程序结束的时候才析构
class seqstack
{
public:
	seqstack(int size =10)//构造函数
	{
		cout <<this << "seqstack()"<<endl;
		_pstack =new int[size];
		_top =-1;
		_size =size;
	}
	~seqstack()//析构函数
	{
		cout <<this << "~seqstack()"<<endl;
		delete []_pstack;//释放数组空间要加[]
		_pstack =nullptr;
	}
	
    void push(int val)
    {
        if(full())
        {
            resize();
        }
        _pstack[++_top]=val;//先自增，再赋值
    }
    void pop()
    {
        if(empty())
        {
            return;
        }
		--_top;
    }
	int top(){return _pstack[_top];}//返回栈顶
	bool empty(){return _top==-1;}//判空
	bool full(){return _top==_size-1;}//判满
private:
	int *_pstack;//动态开辟数组，存储顺序栈的元素
	int _top;//指向栈顶元素的位置
	int _size;//数组扩容的总大小
	void resize()//类成员方法不希望外部调用
	{
		int *ptmp =new int[_size*2];
		for(int i =0;i<_size;++i)
		{
			ptmp[i]=_pstack[i];
		}//memecpy(ptmp,_pstack,sizeof(int)*_size))/realloc都是内存拷贝---涉及到深拷贝和浅拷贝
		_pstack =ptmp;
		_size *=2;
	}
};
```
```C++
SeqStack *ps = new SeqStack(60);
    ps->push(70);
    ps->push(80);
    ps->pop();
    cout << ps->top() << endl;
    delete ps;//需要手动释放 delete先调用析构函数释放堆内存 然后free（ps）
```
```c++
//oop编程实现顺序栈
class seqstack
{
public:
	void init(int size =10)
	{
		_pstack =new int[size];
		_top =-1;
		_size =size;
	}
	void release()
	{
		delete []_pstack;//释放数组空间要加[]
		_pstack =nullptr;
	}
    void push(int val)
    {
        if(full())
        {
            resize();
        }
        _pstack[++_top]=val;//先自增，再赋值
    }
    void pop()
    {
        if(empty())
        {
            return;
        }
		--_top;
    }
	int top(){return _pstack[_top];}//返回栈顶
	bool empty(){return _top==-1;}//判空
	bool full(){return _top==_size-1;}//判满
private:
	int *_pstack;//动态开辟数组，存储顺序栈的元素
	int _top;//指向栈顶元素的位置
	int _size;//数组扩容的总大小
	void resize()//类成员方法不希望外部调用
	{
		int *ptmp =new int[_size*2];
		for(int i =0;i<_size;++i)
		{
			ptmp[i]=_pstack[i];
		}//memecpy(ptmp,_pstack,sizeof(int)*_size))/realloc都是内存拷贝---涉及到深拷贝和浅拷贝
		_pstack =ptmp;
		_size *=2;
	}
};
```
#### 对象的深拷贝和浅拷贝
```c++
seqstack s;//会生成默认构造函数和析构函数，空函数，什么也不做
seqstack s1(10);//调用一个带整型参数的构造函数
seqstack s2 =s1;//由一个已经存在的栈对象，来构造一个栈对象--拷贝构造函数
//1.浅拷贝（不占用外部资源的情况下是没问题的）
//后构造的先析构，s2和s1指向同一个堆内存，但s2析构时先把那块内存释放了，导致s1指向成为了野指针，会运行终止
//导致这种情况的原因是对象的成员变量中有指针，并且指向了对象之外的内存（外部资源）---发生浅拷贝--同一个资源释放两次
//对象默认的拷贝构造是做内存的数据拷贝，也就是成员变量的拷贝，导致拷贝的变量中的指针指向和原对象相同
seqstack s3(s1);//拷贝构造函数，同上

//2. 深拷贝（不止拷贝一份成员变量，如果变量指向外部资源，那么也要单独开辟一段内存来拷贝一份外部资源，使拷贝对象的成员变量指向拷贝的外部资源）
//这样析构的时候各自析构自己指向的堆内存，互不干扰
```
```c++
//自定义拷贝构造函数（深拷贝）
class seqstack
{
public:
	seqstack(int size =10)//构造函数
	{
		cout <<this << "seqstack()"<<endl;
		_pstack =new int[size];
		_top =-1;
		_size =size;
	}
	seqstack(const seqstack & src)//成员变量指向外部资源，不能使用默认构造函数的析构函数进行拷贝
	{
		//_pstack=src._pstack;//浅拷贝
		_pstack =new int [src._size];//复制一份外部资源，让_pstack指向新开辟的堆内存
		for(int i =0;i<=src._top;++i)
		{
			_pstack[i]=src._pstack[i];
		}
		//为什么深拷贝使用for循环而不是memcpy：这里的拷贝数据是整型，所以都能用，是浅拷贝，但是没有问题
		//如果要拷贝的内容不是一个个整型，而是一个个对象，并且其中的指针变量还访问了外部资源，那就不能直接使用memcpy
		//memcpy和realloc都是浅拷贝
		//赋值操作：s2 = s1 如果我们没有给类提供赋值操作，那么会成为默认赋值操作---直接的内存拷贝，浅拷贝：
		//s2.operator=(s1);void operator=(const seqstack &src);没有实现
		_top =src._top;
		_size =src._size;
	}
	void operator=(const seqstack &src)//赋值重载函数
	{
		cout << "operator"<<endl;
		if(this==&src)//防止自己给自己赋值
		{
			return ;
		}
		delete[]_pstack;//需要先释放当前对象占用的外部资源
		_pstack =new int [src._size];//复制一份外部资源，让_pstack指向新开辟的堆内存
		for(int i =0;i<=src._top;++i)
		{
			_pstack[i]=src._pstack[i];
		}
		_top =src._top;
		_size =src._size;
	}
	
	~seqstack()//析构函数
	{
		cout <<this << "~seqstack()"<<endl;
		delete []_pstack;//释放数组空间要加[]
		_pstack =nullptr;
	}
	
    void push(int val)
    {
        if(full())
        {
            resize();
        }
        _pstack[++_top]=val;//先自增，再赋值
    }
    void pop()
    {
        if(empty())
        {
            return;
        }
		--_top;
    }
	int top(){return _pstack[_top];}//返回栈顶
	bool empty(){return _top==-1;}//判空
	bool full(){return _top==_size-1;}//判满
private:
	int *_pstack;//动态开辟数组，存储顺序栈的元素
	int _top;//指向栈顶元素的位置
	int _size;//数组扩容的总大小
	void resize()//类成员方法不希望外部调用
	{
		int *ptmp =new int[_size*2];
		for(int i =0;i<_size;++i)
		{
			ptmp[i]=_pstack[i];
		}//memecpy(ptmp,_pstack,sizeof(int)*_size))/realloc都是内存拷贝---涉及到深拷贝和浅拷贝
		_pstack =ptmp;
		_size *=2;
	}
};
```
### 3.this指针
```c++
//编译器隐式的区分出来成员方法是被哪个对象调用的，在方法传参的时候，会有一个该对象类型的this指针来接收该对象的地址，由此可以使用this->xxx,来表示某对象的成员方法
```
### 4.对象数组
```c++

```
### 5.类作用域
```c++

```
### 6.抽象数据类型
```c++

```
## 4.使用类：
### 1.运算符重载
```c++

```
### 2.计算时间
```c++

```
### 3.友元
```c++

```
### 4.重载
```c++

```
### 5.类的自动转换和强制类型转换
```c++

```
## 5.类和动态内存分配：
### 1.动态内存和类
```c++

```
### 2.在构造函数中使用new
```c++

```
### 3.返回对象
```c++

```
### 4.使用指向对象的指针
```c++

```
### 5.各种技术
```c++

```
### 6.队列模拟
```c++

```
## 6.类继承：
### 1.基类
```c++

```
### 2.继承关系
```c++

```
### 3.多态共有继承
```c++

```
### 4.静态联编和动态联编
```c++

```
### 5.访问控制protected
```c++

```
### 6.抽象基类
```c++

```
### 7.继承和动态内存分配
```c++

```
## 7.c++中的代码重用:
### 1.包含对象成员的类
```c++

```
### 2.私有继承
```c++

```
### 3.多重继承
```c++

```
### 4.类模板
```c++

```
## 8.友元、异常和其他:
### 1.友元
```c++

```
### 2.嵌套类
```c++

```
### 3.异常
```c++

```
### 4.RTTI
```c++

```
### 5.类型转换运算符
```c++

```


