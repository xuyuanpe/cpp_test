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
#### 构造函数的初始化列表
```C++
//初始化顺序，先被声明的先被初始化
//成员变量的初始化和声明顺序有关，和在构造函数列表中的出现顺序无关
#include <iostream>
using namespace std;

class Test
{
public:
    Test(int data = 10) : mb(data), ma(mb) {}
    void show() { cout << "ma:" << ma << " mb:" << mb << endl; }
private:
    int ma;
    int mb;
};

int main()
{
    Test t;
    t.show();
    return 0;
}
//ma先声明，所以先被赋值，此时mb是随机值，然后是mb=data，所以结果是ma=未知，mb=10
```
```C++
//cgood(const char* name, double price, int amount,int y,int m,int d):_date(y,m,d),_price(price),_amount(amount)
//构造函数的初始化列表:可以指定当前对象成员的初始化方式
//cdate 是cgood的一部分 组合


#include <iostream>
using namespace std;
const int NAME_LENGTH = 20;
class cdate
{
public:
	cdate(int y, int m, int d)
	{
		_year = y;
		_month = m;
		_day = d;
	}
	void showdate()
	{
		cout << _year << "/" << _month << "/"<<_day << endl;
	}
private:
	int _year;
	int _month;
	int _day;

};
class cgood
{
private://私有的成员变量
	char _name[NAME_LENGTH];
	double _price;
	int _amount;
	cdate _date;//成员对象---如何把年月日的信息传递给_date

public://共有的成员函数
	void init(const char* name, double price, int amount);//常量字符串不允许用普通指针来接收
	//添加构造函数：
	cgood(const char* name, double price, int amount,int y,int m,int d)
		:_date(y,m,d)
		,_price(price)
		,_amount(amount)// 构造函数的初始化列表,也可以进行简单的赋值
		//指定成员对象的构造方式
		//当前类类型构造函数体
	{
		strcpy_s(_name, name);//这种就放在函数体中
		
	}
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
	_date.showdate();
}
int main()
{
	//如何计算类的内存大小----类的大小只和成员变量有关，和成员方法无关
	//内存计算先找占用最长字节的变量，然后根据编译器的对齐方式排列，最后计算出来

	//一个类可以定义无数个对象，每个对象都有自己的成员变量，但是他们共享一套成员方法
	//	
	//cgood good();//实例化一个对象
	//good.init("面包", 100.5, 100);
	//good.show();
	cgood g("xigua",10.0,10,2021,10,12);
	g.show();
	return 0;
}
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
### 7.类的各种成员方法以及区别
#### 1.静态成员和静态方法
#### 2.常对象调用不了普通方法--相对于const *this== *this 所以要给普通方法后缀加上const变为常成员方法
#### 3.如果常成员方法中还调用了普通方法 那也要把那个方法改为常成员方法
#### 4.只要是只读操作的方法 一律实现成const 常成员方法 这样普通对象可以调用 常对象也可以调用
```C++
//类的成员----成员方法/变量
//普通的成员方法=====》this形参
//1.属于类的作用域
//2.成员方法的调用依赖于对象
//3.可以访问对象的私有变量
//静态成员和静态成员方法=====》不产生this形参
//4.在类中添加一个int count 用来记录商品的总数量---普通的成员变量只能描述一个对象的信息---在构造函数中调用，保证每一次生成对象都会使其值变动
//5.所以可以使用静态成员变量：静态成员变量不纳入类内存--对象在栈中，静态成员变量在数据段中---计算对象大小不考虑静态成员变量
//6.同理我们想不通过对象调用方法，而是通过类名调用，就需要把普通成员方法写成静态成员方法
//7.静态方法无法访问普通的成员变量（因为静态方法没有产生this指针，无法接收对象地址，也不知道访问哪个对象）只能访问不依赖对象的成员变量

#include <iostream>
using namespace std;
const int NAME_LENGTH = 20;
class cdate
{
public:
	cdate(int y, int m, int d)
	{
		_year = y;
		_month = m;
		_day = d;
	}
	void showdate()const
	{
		cout << _year << "/" << _month << "/" << _day << endl;
	}
private:
	int _year;
	int _month;
	int _day;

};
class cgood
{
private://私有的成员变量
	char _name[NAME_LENGTH];
	double _price;
	int _amount;
	cdate _date;//成员对象---如何把年月日的信息传递给_date
	static  int _count;//声明一个静态成员变量，用于给商品计数

public://共有的成员函数
	void init(const char* name, double price, int amount);//常量字符串不允许用普通指针来接收
	//添加构造函数：
	cgood(const char* name, double price, int amount, int y, int m, int d)
		:_date(y, m, d)
		, _price(price)
		, _amount(amount)// 构造函数的初始化列表,也可以进行简单的赋值
		//指定成员对象的构造方式
		//当前类类型构造函数体
	{
		strcpy_s(_name, name);//这种就放在函数体中
		_count++;

	}
	void show();
	
	void show()const;
	
	void setName(char* name) { strcpy_s(_name, name); }//类体内实现的函数，自动处理成内联函数
	void setPrice(double price) { _price = price; }
	void setAmount(int amount) { _amount = amount; }
	const char* getName() { return _name; }
	double getPrice() { return _price; }
	int getAmount() { return _amount; }
	static void show_count() { cout << "count:" << _count << endl; }

};
//类外定义：想要处理成内联函数需要加上关键字inline
void cgood::init(const char* name, double price, int amount)//方法前面要加上类的作用域（返回值后面，函数名前面）
{
	strcpy_s(_name, name);//strcpy不安全
	_price = price;
	_amount = amount;

}
void cgood::show()//普通成员方法
{
	cout << "name:" << _name << endl;
	cout << "price:" << _price << endl;
	cout << "amount:" << _amount << endl;
	_date.showdate();
}
void cgood::show()const //const cgood *this ==>常成员方法
{
	cout << "name:" << _name << endl;
	cout << "price:" << _price << endl;
	cout << "amount:" << _amount << endl;
	_date.showdate();
}
//类外定义_count:
int cgood::_count = 0;//类外定义，静态成员只能在类外定义并初始化，可以在类内进行声明
int main()
{
	//如何计算类的内存大小----类的大小只和成员变量有关，和成员方法无关
	//内存计算先找占用最长字节的变量，然后根据编译器的对齐方式排列，最后计算出来

	//一个类可以定义无数个对象，每个对象都有自己的成员变量，但是他们共享一套成员方法

	cgood g1("商品1", 10.0, 10, 2021, 10, 12);
	cgood g2("商品2", 10.0, 10, 2021, 10, 12);
	cgood g3("商品3", 10.0, 10, 2021, 10, 12);
	cgood g4("商品4", 10.0, 10, 2021, 10, 12);
	cgood g5("商品5", 10.0, 10, 2021, 10, 12);
	g1.show_count();//虽然可以打印所有信息，但是调用的是某个具体成员中的方法，从代码的角度看怪怪的，存在this指针
	//因为这是个普通的成员方法，编译器在调用的时候会传入this指针，对于特定的对象的地址
	//cgood：：show_conut()这种调用又没有对象地址传入，不合法
	//所以我们不能把这个方法写成普通的成员方法然后使用对象去调用它
	//要是有类名去调用这个方法，应该把这个方法写成静态成员方法static---无this指针
	cgood::show_count();
	g1.show();
	g2.show();
	g3.show();
	g4.show();
	g5.show();
	//需求：统计所有商品的总数量
	const cgood g6("镇店之宝",100.0,10,2025,10,10);//const 对象信息不允许修改
	//常量对象调用普通方法
	//g6.show()--->不兼容普通成员方法，因为有const不能泄露给普通this指针
	cgood::show_count();
	g6.show();
	return 0;
}
```
### 8.指向类成员的指针
#### 1.指向成员变量的指针
```C++
/*
1.
*/
#include <iostream>
using namespace std;
class test
{
public:
	void func() { cout << "call test::func" << endl; }
	static void static_func() { cout << "test::static_func" << endl; }
	int ma;
	static int mb;
private:

};
// 关键：类外定义静态成员变量，分配实际内存（必须加这一行）
int test::mb = 0;
int main()
{
	//1.我能不能通过一个普通的指针指向一个类的成员变量（在脱离对象的实体情况下----不行，不能脱离对象）
	//int* p = &test::ma;//会报错，编译器认为两者的类型是不一致的
	//int test::* p = &test::ma;//这样类型就相同了，可以运行
	//*p = 20;
	//会报错，*解引用作用操作数是指针，但编译器认为p的类型是int test::*
	//其实也就是说这个ma一定是依赖于对象存在，脱离了对象再访问ma没有任何意义

	//2.分别创建在栈上的对象和在堆上的对象
	test t1;//栈自动创建
	test* t2 = new test();//堆
	int test::* p = &test::ma;
	t1.*p = 20;
	cout << t1.ma << endl;//运行没有问题
	t2->*p = 30;
	cout << t2->ma << endl;//t2是个指针，也没有问题
	//3.静态成员变量不依赖于对象，可以通过普通指针指向静态成员变量
	int* q = &test::mb;
	*q = 40;
	cout << test::mb << endl;//这句话没问题，但是静态成员变量在类中声明之后，一定要定义和初始化


	delete t2;
	return 0;
}
```
#### 2.指向成员方法的指针
```C++
/*
1.普通成员方法：依赖对象
2.静态成员方法：不依赖对象
*/
#include <iostream>
using namespace std;
class test
{
public:
	void func() { cout << "call test::func" << endl; }
	static void static_func() { cout << "test::static_func" << endl; }
	int ma;
	static int mb;
private:

};
// 关键：类外定义静态成员变量，分配实际内存（必须加这一行）
int test::mb = 0;
int main()
{
	//指向成员方法的指针
	test t1;//栈自动创建
	test* t2 = new test();//堆
	//void (*p_func)() = &test::func;//运行失败
	//1.指向普通类成员方法的指针
	void(test:: * p_func)() = &test::func;//可以运行，但是脱离对象就没有意义
	(t1.*p_func)();//前面要加括号
	(t2->*p_func)();
	//2.指向静态成员方法的指针
	void (*p_static_func)() = &test::static_func;
	(*p_static_func)();
	return 0;
}
```
## 4.使用类：
### 1.运算符重载
```c++
/*
1.运算符重载的意义：使对象的运算表现得跟编译器内置类型一样
template<typename T>
T sum(T a,T b)
{
	return a+b //a.operator+(b)---->b作为参数，a调用自己的+函数来处理b 
}
*/
//复数类：
#include<iostream>
using namespace std;
//运算符重载
//template<typename T>
class CComplex //复数类
{
public:
	//这里因为构造函数有默认值，所以传一个数看一看作是给实部赋值，虚部默认为0
	CComplex(int r =0,int i=0 ):mreal(r),mimage(i)
	{
		cout << "call CComplex" << endl;
	}
	//告诉编译器怎么做CComplex类的加法
	//comp1,comp2参与加法，得到comp3，但本身一定是保持不变
	//所以在重载函数中我们要定义一个新对象来接收两个对象相加的结果，最后返回这个新对象
	CComplex operator+(const CComplex & other)//====>返回值是CComplex类型，传入的参数也是CComolex类型
	{
		CComplex comp;//这样定义的comp属于这个函数的局部对象，在函数运行结束后栈帧回退，该临时对象内存会被删除
		comp.mreal = this->mreal + other.mreal;
		comp.mimage = this->mimage + other.mimage;
		return comp;
		//return CComplex(this->mreal + other.mreal,this->mimage + other.mimage);
	}
	CComplex operator++(int)
	{
		//CComplex comp = *this;
		//mreal += 1;
		//mimage += 1;
		//return comp;//先赋值
		return CComplex(mreal++, mimage++);//少了comp对象构造和析构的过程
	}
	CComplex operator++()
	{
		//mreal += 1;
		//mimage += 1;
		//return *this;//先++
		return CComplex(++mreal, ++mimage);
	}
	void operator+=(const CComplex& other)
	{
		mreal += other.mreal;
		mimage += other.mimage;
	}
	~CComplex()
	{
		cout << "call ~CComplex" << endl;
 	}
	void show()
	{
		cout << "real:" << mreal << endl;
		cout << "image:" << mimage << endl;
	}

private:
	int mreal;//实部
	int mimage;//虚部
	friend CComplex operator+(const CComplex& lhs, const CComplex& rhs);
	friend ostream& operator<<(ostream& out, const CComplex& other);
	friend istream& operator>>(istream& input, CComplex& other);
};
CComplex operator+(const CComplex& lhs, const CComplex& rhs)//将其定义为友元函数，用以访问私有变量
{
	return CComplex(lhs.mreal + rhs.mreal, lhs.mimage + rhs.mimage);//私有成员变量不可类外访问
}
ostream& operator<<(ostream& out, const CComplex& other)
{
	out << "mreal: " << other.mreal <<" " << "mimage: " << other.mimage << endl;
	return out;
}
istream& operator>>(istream& input, CComplex& other)
{
	input >> other.mreal >> other.mimage;
	return input;
}




//template<typename T>
//void show(T a)//如果T是CComplex类型该怎么办 让对象也支持cout<< <<endl;
//{
//	cout << a << endl;
//}

int main()
{
	CComplex comp1(10, 10);
	CComplex comp2(20, 20);
	// 编译器首先再comp1对象里找有没有+这个成员方法--->comp1.operator+(comp2)加法运算符的重载函数
	//CComplex comp3 = comp1 + comp2;
	//对象和整数（作为复数的实部）相加的情况又会怎样处理 能不能把这个整数转换成复数类型CComplex
	//CComplex comp4 = comp1 + 20;//===>comp1.operator+(complex(20)):隐含了一个类型转换
	/*编译器做对象运算时，会调用对象的运算符重载函数（优先调用成员方法）
	* 如果没有合适的成员方法，就在全局作用域找合适的运算符重载函数 
	* ：：operator+（30，comp1）----全局
	*/
	//===>没有定义这样的重载运算符，不成立,添加友元函数后，30会被处理成实部为30，虚部为0的临时对象
	CComplex comp5 = 30 + comp1;
	//comp3.show();
	//comp4.show();
	//comp5.show();
	//没有定义这个++运算符的重载函数 前置先++再赋值，后置先赋值再++
	//++ -- 是单目运算符 只有一个操作数 区分不了前置和后缀
	//C++规定operator++()----前置   operator++(int)----后缀
	//CComplex operator++(int)---后缀
	comp5 = comp1++;
	comp1.show();
	comp5.show();
	//CComplex operator++()----前置 
	comp5 = ++comp1;
	comp1.show();
	comp5.show();
	//void comp1.operator+=(comp2)
	comp1 += comp2;
	comp1.show();
	//对象在右边，不能作为成员方法
	//定义为全局方法 ：：operator<<(cout,comp1)
	//ostream& operator<<(ostream&, const CComplex &)
	cin >> comp1 >> comp2;
	cout << comp1 << endl;
	cout << comp2 << endl;
	return 0;
}
```
### 2.计算时间
```c++

```
### 3.友元
```c++

```
### 4.模拟C++实现string类代码
```c++

```
### 5.类的自动转换和强制类型转换
```c++
/*
在 C++ 中，类的类型转换分为自动类型转换（隐式转换） 和强制类型转换（显式转换），
核心是通过类的构造函数或转换函数实现自定义类型与其他类型（内置类型 / 其他类）的转换

*/
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
#### 1.任意数据类型都可以使用的顺序栈
```c++
//类模板===>实例化 ====>模板类
//模板顺序栈
#include <iostream>
#include <typeinfo>  // 必须包含此头文件

using namespace std;
template<typename T>
class seqstack  //模板名称，还没有创建具体的类 格式为模板名 +<类型参数列表> =类名称
{
public:
	seqstack<T>(int size = 10)//构造--<T>可以省略
		:_pstack(new T [size])
		,_top(0)
		,_size(size)//成员变量的初始化写在构造函数参数列表中
	{
		cout << "call seqstack<T>create :"<< typeid(*_pstack).name() << endl;
	}
	~seqstack<T>()//析构--<T>可以省略
	{
		cout << "call ~seqstack<T>" << endl;
		delete[]_pstack;
		_pstack = nullptr;
	}
	seqstack<T>(const seqstack<T> &stack) //拷贝构造----<T>可以省略
		:_top(stack._top)
		,_size(stack._size)
	{
		_pstack = new T[_size];
		for (int i = 0; i < _top; ++i)
		{
			_pstack[i] = stack._pstack[i];
		}
	}
	seqstack<T>& operator=(const seqstack<T>& stack)//赋值运算符的重载函数
	{
		if (this == &stack)
		{
			return *this;
		}
		delete[]_pstack;
		_top = stack._top;
		_size = stack._size;
		_pstack = new T[_size];
		for (int i = 0; i < _top; ++i)
		{
			_pstack[i] = stack._pstack[i];
		}
		return *this;

	}
	void push(const T& val)
	{
		if (full())
		{
			expand();
		}
		_pstack[_top++] = val;

	}
	void pop()
	{
		if (empty())
		{
			return;
		}
		--_top;
	}
	T top()const
	{
		if (empty())
		{
			throw "stack is empty!";//抛出异常也代表函数逻辑结束返回
		}
		return _pstack[_top - 1];

	}
	bool full()const { return _top == _size; }
	bool empty()const { return _top == 0; }

private:
	T* _pstack;
	int _top;
	int _size;
	void expand()//扩容
	{
		T* ptmp = new T[_size * 2];
		for (int i = 0; i < _top; ++i)
		{
			ptmp[i] = _pstack[i];
		}
		delete[]_pstack;
		_pstack = ptmp;
		_size *= 2;
	}
};

int main()
{
	seqstack<int> s1;
	s1.push(20);
	s1.push(30);
	s1.push(40);
	s1.push(50);
	s1.pop();
	cout << s1.top() << endl;

	seqstack<char> s2;
	seqstack<double> s3;
	return 0;
}
```
#### 2.类模板实现vector顺序容器
```C++
#include<iostream>
#include <typeinfo>
//#include<vector>
using namespace std;
//vector 向量容器
//容器
//空间配置器 allocator
template <typename T>
class vector
{
public:
	vector(int size = 10)
	{
		_first = new T[size];
		_last = _first;
		_end = _first + size;
		cout << typeid(*_first).name() << endl;
	}
	~vector()
	{
		delete[]_first;
		_first = _last = _end = nullptr;
	}
	vector(const vector<T>& other)
	{
		int size = other._end - other._first;
		_first = new T[size];
		int len = other._last - other._first;
		for (int i = 0; i < len; ++i)
		{
			_first[i] = other._first[i];
		}
		_last = _first + len;
		_end = _first + size;
	}
	vector<T>& operator=(const vector<T>& other)
	{
		if (this == &other)
		{
			return *this;
		}
		delete[]_first;
		int size = other._end - other._first;
		_first = new T[size];
		int len = other._last - other._first;
		for (int i = 0; i < len; ++i)
		{
			_first[i] = other._first[i];
		}
		_last = _first + len;
		_end = _first + size;
		return *this;
	}
	void push_back(const T &val)//向容器末尾添加元素
	{
		if (full())
		{
			expand();
		}
		*_last++ = val;
	}
	void pop_back()//从容器末尾删除元素
	{
		if (empty())
		{
			return;
		}
		--_last;
	}
	T back()const//返回容器末尾元素值
	{
		return *(_last - 1);
	}
	bool full()const
	{
		return _last == _end;
	}
	bool empty()const
	{
		return _first == _last;
	}
	int size()
	{
		return _last - _first;
	}
	private:
	T* _first;//指向第一个有效元素的指针，数组的起始位置 
	T* _last;//指向最后一个有效元素的后继
	T* _end;//指向整个数组最后一个位置的后继
	void expand()
	{
		int size = _last - _first;
		T * ptmp = new T[2 * size];
		for (int i = 0; i < size; ++i)
		{
			ptmp[i] = _first[i];

		}
		delete[]_first;
		_first = ptmp;
		_last = _first + size;
		_end = _first + 2 * size;

	}
};
int main()
{
	vector<int>vec;
	for (int i = 0; i < 20; ++i)
	{
		vec.push_back(rand() % 100);
	}
	while (!vec.empty())
	{
		cout << vec.back() << " ";
		vec.pop_back();
	}
	cout << endl;
	return 0;
}
```
#### 3.空间配置器
##### allocator的作用：
```C++
/*1.使用自己编写的vector模板类来创建类类型的向量容器，new操作符在生成时开辟空间并且构造对象，不断的调用构造函数和析构函数
  2.由于析构时使用delete操作符，相当于把指针指向的每一个元素都当成对象析构一遍
  3.而我们只需要他开辟空间，我们自己放入元素的个数和创建模板的大小可能是不一样的
  4.所以，我们真正的需求是：构造函数时开辟空间和构造函数分开处理 析构容器有效的元素，释放整个底层的内存空间
*/
/*
容器空间配置器的作用:
内存开辟 内存释放 对象构造 对象析构
*/
#include<iostream>
#include <typeinfo>
//#include<vector>
using namespace std;
//vector 向量容器
//容器
//空间配置器 allocator：内存开辟 内存释放 对象构造 对象析构
//自己实现空间配置器
template<typename T>
class Allocator
{
public:
	T* allocate(size_t size)//只负责内存开辟
	{
		return(T *)malloc(sizeof(T) * size);
	}
	void deallocate(void* p)//负责内存释放
	{
		free(p);
	}
	void construct(T* p, const T& val)//负责对象构造
	{
		new (p)T(val);//定位new
	}
	void destroy(T* p)//负责析构对象
	{
		p->~T();//代表了T类型的析构函数
	}
};
template <typename T,typename Alloc= Allocator<T>>
class vector
{
public:
	vector(int size = 10,const Alloc &alloc = Allocator<T>())
		:_allocator(alloc)
	{
		//需要把内存开辟和对象构造分开处理
		//_first = new T[size];
		_first = _allocator.allocate(size);
		_last = _first;
		_end = _first + size;
		//cout << typeid(*_first).name() << endl;
	}
	~vector()
	{
		//析构容器有效的元素，然后释放_first指针指向的堆内存
		//delete[]_first;
		for (T* p = _first; p != _last; ++p)
		{
			_allocator.destroy(p);
		}
		_allocator.deallocate(_first);
		_first = _last = _end = nullptr;
	}
	vector(const vector<T>& other)//拷贝构造
	{
		int size = other._end - other._first;
		//_first = new T[size];
		_first = _allocator.allocate(size);
		int len = other._last - other._first;
		for (int i = 0; i < len; ++i)
		{
			//_first[i] = other._first[i];
			_allocator.construct(_first + i, other._first[i]);
		}
		_last = _first + len;
		_end = _first + size;
	}
	vector<T>& operator=(const vector<T>& other)//赋值重载函数
	{
		if (this == &other)
		{
			return *this;
		}
		//delete[]_first;
		for (T *p = _first; p != _last; ++p)
		{
			_allocator.destroy(p);//把_first指针指向的数组的有效元素进行析构操作
		}
		_allocator.deallocate(_first);
		int size = other._end - other._first;
		//_first = new T[size];
		_first = _allocator.allocate(size);
		int len = other._last - other._first;
		for (int i = 0; i < len; ++i)
		{
			//_first[i] = other._first[i];
			_allocator.construct(_first + i, other._first[i]);
		}
		_last = _first + len;
		_end = _first + size;
		return *this;
	}
	void push_back(const T &val)//向容器末尾添加元素
	{
		if (full())
		{
			expand();
		}
		//*_last++ = val;
		//在last指针位置构造一个值为val的对象
		_allocator.construct(_last, val);
		_last++;
	}
	void pop_back()//从容器末尾删除元素，
	{
		if (empty())
		{
			return;
		}
		//--_last;
		//析构删除的元素再左移
		--_last;
		_allocator.destroy(_last);


	}
	T back()const//返回容器末尾元素值
	{
		return *(_last - 1);
	}
	bool full()const
	{
		return _last == _end;
	}
	bool empty()const
	{
		return _first == _last;
	}
	int size()
	{
		return _last - _first;
	}
	private:
	T* _first;//指向第一个有效元素的指针，数组的起始位置 
	T* _last;//指向最后一个有效元素的后继
	T* _end;//指向整个数组最后一个位置的后继
	Alloc _allocator;//定义容器的空间配置器对象
	void expand()
	{
		int size = _last - _first;
		//T * ptmp = new T[2 * size];
		T* ptmp = _allocator.allocate(2 * size);

		for (int i = 0; i < size; ++i)
		{
			_allocator.construct(ptmp + i, _first[i]);
			//ptmp[i] = _first[i];

		}
		//delete[]_first;
		for (T* p = _first; p != _last; ++p)
		{
			_allocator.destroy(p);
		}
		_allocator.deallocate(_first);
		_first = ptmp;
		_last = _first + size;
		_end = _first + 2 * size;

	}
};
class test
{
public:
	test()
	{
		cout << "call test" << endl;
	}
	~test()
	{
		cout << "call ~test" << endl;
	}
	test(const test& other) { cout << "test(const test&)" << endl; }
private:
};
int main()
{
	/*vector<int>vec;
	for (int i = 0; i < 20; ++i)
	{
		vec.push_back(rand() % 100);
	}
	while (!vec.empty())
	{
		cout << vec.back() << " ";
		vec.pop_back();
	}
	cout << endl;*/
	test t1, t2, t3;
	
	cout << "--------------" << endl;
	vector<test> t;//构造十个对象（默认）
	t.push_back(t1);
	t.push_back(t2);
	t.push_back(t3);
	cout << "--------------" << endl;
	t.pop_back();//只需要析构对象，把对象的析构和内存的释放也分离开，因为这块内存是属于数组的，使用delete会把这一块空间给释放
	cout << "--------------" << endl;
	//理论上，vector<test> t只是开辟了内存，随后我们的push_back()执行，在内存中开始创建对象
	//然而现在的逻辑是，我们执行完vector<test> t后，他开辟空间并且创建了对象，我们执行的push_back变成了赋值
	//如果我们的执行pop_back也就是将_last向左移，而test这个对象还有指针指向外部空间，那么test对象指向的外部空间就得不到释放
	//所以在容器的空间管理中，不能直接用new和delete
	return 0;
}
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


