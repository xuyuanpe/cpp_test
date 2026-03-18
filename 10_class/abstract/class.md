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
#include<iostream>
#include<string>
using namespace std;
class String //自己实现一个字符串对象
{
public:
	String(const char* p = nullptr)
	{
		if (p != nullptr)
		{
			_pstr = new char[strlen(p) + 1];//strlen是计算有效字符串长度，而c格式的字符串在字符后面还有个尾0
			strcpy(_pstr, p);
		}
		else
		{
			_pstr = new char[1];
			*_pstr = '\0';//避免每次使用都要判断其是否为nullptr
		}
	}
	~String()
	{
		delete[]_pstr;
		_pstr = nullptr;
	}
	//拷贝构造函数
	String(const String& other)
	{
		_pstr = new char[strlen(other._pstr) + 1];
		strcpy(_pstr, other._pstr);

	}
	//赋值运算符的重载
	String& operator=(const String& other)
	{
		if (this == &other)
		{
			return *this;
		}
		else
		{
			delete[]_pstr;
			_pstr = new char[strlen(other._pstr) + 1];
			strcpy(_pstr, other._pstr);
			return *this;
		}
	}
	bool operator>(const String &other)const
	{
		return strcmp(_pstr, other._pstr)>0;
	}
	bool operator<(const String& other)const
	{
		return strcmp(other._pstr, _pstr)>0;
	}
	bool operator==(const String& other)const
	{
		return strcmp(other._pstr, _pstr)==0;
	}
	int length()const { return strlen(_pstr); }//返回有效字符个数
	//char& str6.operator[](i)===>str6[i]===>char &
	char& operator[](int index) { return _pstr[index]; }//普通方法---可修改
	const char& operator[](int index)const { return _pstr[index]; }//常方法---仅读取
	//返回char*类型
	const char* c_str()const { return _pstr; }
private:
	char* _pstr;
	friend ostream& operator<<(ostream& out, const String& other);
	friend String operator+(const String& lhs, const String& rhs);
};
//全局的加法重载函数
String operator+(const String &lhs,const String &rhs)//没有delete过，会造成内存泄漏
{
	////strcat(lhs._pstr,rhs._pstr)---->原来的字符串长度不够需要重新构造
	//char* ptmp = new char[strlen(lhs._pstr) + strlen(rhs._pstr)+1];
	//strcpy(ptmp, lhs._pstr);
	//strcat(ptmp, rhs._pstr);
	//String tmp(ptmp);
	//delete[]ptmp;
	//return tmp;//思考一下如何能使效率更高
	String tmp;
	tmp._pstr = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
	strcpy(tmp._pstr, lhs._pstr);
	strcat(tmp._pstr, rhs._pstr);
	return tmp;//临时对象

}
//全局的打印函数
ostream& operator<<(ostream& out, const String& other)
{
	out << other._pstr;
	return out;
}
//全局的输入函数
//istream& operator>>(istream& in, const String& other)
//{
//
//}
int main()
{
#if 0
	String str1;
	String str2 = "aaa";
	String str3 = "bbb";
	String str4 = str2 + str3;//拼接---》提供了一个全局的+运算符重载函数，既可以对象和对象相加，也可以对象和字符串相加
	String str5 = str2 + "ccc";
	String str6 = "ddd" + str2;
	cout << str1 << endl;
	cout << str2 << endl;
	cout << str3 << endl;
	cout << str4 << endl;
	cout << str5 << endl;
	cout << str6 << endl;
	if (str5 > str6)
	{
		cout << str5 << ">" << str6 << endl;
	}
	else
	{
		cout << str5 << "<" << str6 << endl;
	}
	int len = str6.length();
	int size = sizeof(str6);//计算的是string类本身的大小
	int size2 = sizeof("aaabbb");//--->7==》c风格的字符串表示是字符个数后加上'\0'结束符
	cout << "len= " << len << endl;
	cout << "size =" << size << endl;
	cout << "size2 =" << size2 << endl;
	for (int i = 0; i < len; ++i)
	{
		cout << str6[i] << " ";//对象当成数组名来使用
	}
	cout << endl;

	//string--->char*
	//将string里的字符串拷贝到buf里面
	char buf[1024] = { 0 };
	strcpy(buf, str6.c_str());//c_str能将对象的字符串返回为char*类型
	cout << "buf:" << buf << endl;
#endif
	return 0;
}
```
### 5.string字符串对象的迭代器iterator实现
```C++
//迭代器可以透明的访问容器内部的元素的值
//每一种容器都有自己的迭代器
//++运算符重载和*运算符重载
//泛型算法接收的都是迭代器---泛型算法--全局函数---给所有容器用的----有一套统一的操作可以遍历所以容器的元素---用迭代器
#include<iostream>
//#include<string>
using namespace std;
class String //自己实现一个字符串对象
{
public:
	String(const char* p = nullptr)
	{
		if (p != nullptr)
		{
			_pstr = new char[strlen(p) + 1];//strlen是计算有效字符串长度，而c格式的字符串在字符后面还有个尾0
			strcpy(_pstr, p);
		}
		else
		{
			_pstr = new char[1];
			*_pstr = '\0';//避免每次使用都要判断其是否为nullptr
		}
	}
	~String()
	{
		delete[]_pstr;
		_pstr = nullptr;
	}
	//拷贝构造函数
	String(const String& other)
	{
		_pstr = new char[strlen(other._pstr) + 1];
		strcpy(_pstr, other._pstr);

	}
	//赋值运算符的重载
	String& operator=(const String& other)
	{
		if (this == &other)
		{
			return *this;
		}
		else
		{
			delete[]_pstr;
			_pstr = new char[strlen(other._pstr) + 1];
			strcpy(_pstr, other._pstr);
			return *this;
		}
	}
	bool operator>(const String &other)const
	{
		return strcmp(_pstr, other._pstr)>0;
	}
	bool operator<(const String& other)const
	{
		return strcmp(other._pstr, _pstr)>0;
	}
	bool operator==(const String& other)const
	{
		return strcmp(other._pstr, _pstr)==0;
	}
	int length()const { return strlen(_pstr); }//返回有效字符个数
	//char& str6.operator[](i)===>str6[i]===>char &
	char& operator[](int index) { return _pstr[index]; }//普通方法---可修改
	const char& operator[](int index)const { return _pstr[index]; }//常方法---仅读取
	//返回char*类型
	const char* c_str()const { return _pstr; }
	//实现String的迭代器：提供一种统一的方式，来透明的遍历访问容器
	class iterator
	{
	public:
		iterator(char* p = nullptr):_pit(p)
		{

		}
		bool operator!=(const iterator& it)
		{
			return _pit != it._pit; 
		}
		void operator++()
		{
			++ _pit ;
		}
		char& operator*() { return *_pit; }
	private:
		char* _pit;
	};
	
	iterator begin() { return iterator(_pstr); }//返回容器底层首元素的迭代器表示
	iterator end() { return iterator(_pstr + length()); }//返回的是容器底层尾元素后继的迭代器表示
private:
	char* _pstr;
	friend ostream& operator<<(ostream& out, const String& other);
	friend String operator+(const String& lhs, const String& rhs);
};
//全局的加法重载函数
String operator+(const String &lhs,const String &rhs)//没有delete过，会造成内存泄漏
{
	////strcat(lhs._pstr,rhs._pstr)---->原来的字符串长度不够需要重新构造
	//char* ptmp = new char[strlen(lhs._pstr) + strlen(rhs._pstr)+1];
	//strcpy(ptmp, lhs._pstr);
	//strcat(ptmp, rhs._pstr);
	//String tmp(ptmp);
	//delete[]ptmp;
	//return tmp;//思考一下如何能使效率更高
	String tmp;
	tmp._pstr = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
	strcpy(tmp._pstr, lhs._pstr);
	strcat(tmp._pstr, rhs._pstr);
	return tmp;//临时对象

}
//全局的打印函数
ostream& operator<<(ostream& out, const String& other)
{
	out << other._pstr;
	return out;
}
//全局的输入函数
//istream& operator>>(istream& in, const String& other)
//{
//
//}
int main()
{
	String str1 = "hello";//str1--->容器
	//遍历？
	//容器的迭代器 begin---第一个元素 end----最后一个元素的后继
	String::iterator it = str1.begin();
	//auto it =str1.begin();自动类型推导
	for (; it != str1.end(); ++it)
	{
		cout << *it << " ";

	}
	cout << endl;
	//c++ 11 foreach方式实现遍历---->基于可用的begin和end函数，底层仍然是通过迭代器遍历的
	for (char ch : str1)
	{
		cout << ch << " ";
	}
	cout<< endl;

	return 0;
}
```
### 6.vector对象的迭代器实现
```C++
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
	T& operator[](int index)
	{ 
		if (index < 0 || index >= size())
		{
			throw "OUT OF RANGE EXCEPTION !";
		}
			
		return _first[index]; 
	}
	//迭代器一般实现为容器的嵌套类型
	class iterator 
	{
	public:
		iterator(T *p =nullptr):_ptr(p){ }
		bool operator!=(const iterator& it)const
		{
			return _ptr != it._ptr;
		}
		void operator++()
		{
			_ptr++;
		}
		T& operator*() { return *_ptr; }
		const T& operator*()const { return *_ptr; }
	private:
		T* _ptr;
	};
	//begin end
	iterator begin() { return iterator(_first); }
	iterator end() { return iterator(_last); }
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
	///*vector<int>vec;
	//for (int i = 0; i < 20; ++i)
	//{
	//	vec.push_back(rand() % 100);
	//}
	//while (!vec.empty())
	//{
	//	cout << vec.back() << " ";
	//	vec.pop_back();
	//}
	//cout << endl;*/
	//test t1, t2, t3;
	//
	//cout << "--------------" << endl;
	//vector<test> t;//构造十个对象（默认）
	//t.push_back(t1);
	//t.push_back(t2);
	//t.push_back(t3);
	//cout << "--------------" << endl;
	//t.pop_back();//只需要析构对象，把对象的析构和内存的释放也分离开，因为这块内存是属于数组的，使用delete会把这一块空间给释放
	//cout << "--------------" << endl;
	////理论上，vector<test> t只是开辟了内存，随后我们的push_back()执行，在内存中开始创建对象
	////然而现在的逻辑是，我们执行完vector<test> t后，他开辟空间并且创建了对象，我们执行的push_back变成了赋值
	////如果我们的执行pop_back也就是将_last向左移，而test这个对象还有指针指向外部空间，那么test对象指向的外部空间就得不到释放
	////所以在容器的空间管理中，不能直接用new和delete
	vector<int>vec;
	for (int i = 0; i < 20; ++i)
	{
		vec.push_back(rand() % 100 + 1);
	}
	auto it = vec.begin();
	for (; it != vec.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;
	for (auto i : vec)
	{
		cout << i << " ";
	}
	cout << endl;

	return 0;
}
```
### 7.容器的迭代器失效问题
```C++
#include<iostream>
#include<vector>
/*
* 1.迭代器为什么会失效：
* a.当容器调用arase ，当前位置到容器末尾所有迭代器全部失效
* b.当容器调用insert，插入位置到容器末尾所有迭代器全部失效
* c.扩容：全部失效
* 2.为什么会失效
* 3.怎么解决迭代器失效问题：迭代器更新
*/
using namespace std;
int main()
{
	vector<int>vec;
	for (int i = 0; i < 20; ++i)
	{
		vec.push_back(rand() % 100 + 1);
	}
	for (int i : vec)
	{
		cout << i << " ";
	}
	cout<<endl;
#if 1
	//1.删除vec容器中的所有偶数
	auto it = vec.begin();
	while(it != vec.end())//不能用++it，因为删除以后，后面的元素会往前挪
	{
		if ((*it) % 2 == 0)
		{
			//迭代器失效：发生在第一次调用erase以后--->容器缩小
			//erase会返回一个新的迭代器
			//更新迭代器
			it = vec.erase(it);//从容器中删除it当前指向元素 insert --->insert(it,val);
			//break;//只删除一个
		}
		else
		{
			++it;
		}
	}
	for (int i : vec)
	{
		cout << i << " ";
	}
	cout << endl;
	
#endif
#if 0
	//2.给vec容器所有的偶数前面添加一个数（比偶数值小1）
	auto it = vec.begin();
	for (; it != vec.end(); ++it)
	{
		if ((*it) % 2 == 0)
		{
			//迭代器失效----容器增大
			vec.insert(it, *it - 1);
			//break;
		}
	}
#endif
	return 0;
}
```
```C++
//自己实现更新迭代器
```
### 8.类的自动转换和强制类型转换
```c++
/*
在 C++ 中，类的类型转换分为自动类型转换（隐式转换） 和强制类型转换（显式转换），
核心是通过类的构造函数或转换函数实现自定义类型与其他类型（内置类型 / 其他类）的转换

*/
```
### 9.实现对象池
```C++
#include<iostream>

//对象池 池化技术
using namespace std;

template<typename T>
class Queue
{
public:
	Queue()
	{
		_front = _rear = new QueueItem();
	}
	~Queue()
	{
		QueueItem* cur = _front;
		while (cur!= nullptr)
		{
			_front = _front->_next;
			delete cur;
			cur = _front;
		}
	}
	void push(const T& val)//入队操作
	{
		QueueItem* item = new QueueItem(val);
		_rear->_next = item;
		_rear = item;
	}
	void pop()
	{
		if (empty())
		{
			return;
		}
		QueueItem* first = _front->_next;
		_front->_next = first->_next;
		
		if (_front->_next == nullptr)
		{
			_rear = _front;
		}
		delete first;
	}
	T front()const
	{
		return _front->_next->_data;
	}
	bool empty()const
	{
		return _front == _rear;
	}
private:
	struct QueueItem //产生一个QueueItem的对象池（1000个QueueItem）
	{
		QueueItem(T data = T()) :_data(data),_next(nullptr){}
		//给QueueItem提供自定义内存管理 也是在堆上开辟的
		void* operator new(size_t size)
		{
			if (free_pool_ptr == nullptr)
			{
				free_pool_ptr = (QueueItem*)new char[POOL_ITEM_SIZE*sizeof(QueueItem)];
				QueueItem* p = free_pool_ptr;
				for (; p < (free_pool_ptr + POOL_ITEM_SIZE - 1); ++p)
				{
					p->_next = p + 1;
				}
				p->_next = nullptr;
			}
			QueueItem* p = free_pool_ptr;
			free_pool_ptr = free_pool_ptr->_next;
			return p;
			
		}
		void operator delete(void* ptr)
		{
			QueueItem* p = (QueueItem*)ptr;
			p->_next = free_pool_ptr;
			free_pool_ptr = p;
		}
		T _data;
		QueueItem* _next;
		static QueueItem* free_pool_ptr;//指向对象池的指针
		static const int POOL_ITEM_SIZE =100000;
	};
	
	QueueItem* _front;//指向头节点
	QueueItem* _rear;//指向队尾
};
template<typename T>
typename Queue<T>::QueueItem* Queue<T>::QueueItem::free_pool_ptr = nullptr;
int main()
{
	Queue<int> que;
	//数量多起来，短时间内会大量的执行开辟内存和释放内存的操作，如果之间产生一个QueueItem的对象池，后面统一的进行内存的管理
	//就可以实现程序性能的优化
	for (int i = 0; i < 1000; ++i)
	{
		que.push(i);
		que.pop();
	}
	cout << que.empty() << endl;
	return 0;
}
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
```C++
/* 
1.继承的本质和原理
2.派生类的构造原理
3.重载 覆盖 隐藏
4.静态绑定和动态绑定
5.多态 vfptr vftable
6.抽象类的设计原理
7.多重继承以及问题
8.虚基类 vbptr vbtable
9.RTTI
10.C++四种类型强转
11.继承多态常见问题 
*/
```
### 1.基类

```c++

```
### 2.继承关系
```c++
#include<iostream>
using namespace std;
/*
* 1.继承的本质和原理
* a：代码的复用
* 2.类和类之间的关系 组合 继承
* 3.访问权限：权限不能大于继承方式 比如基类的public被子类以protected方式继承，那就按protected的权限处理
* 4.private成员可以被继承，但是派生类无法直接访问
* 5.保护成员和私有成员的区别 ：基类的成员想被派生类访问，又不想被外部访问
* 6.class定义派生类，默认为私有private struct 定义派生类 默认是public
* 7.派生类的构造过程：
* 继承的方式      基类的访问限定      派生类的访问限定      外部访问限定
* public：
*                 public              public--->ok        public-->ok
*				  protected           protected--->ok     no
*                 private             no 不可见            no
* protected： 
*                 public			  protected            no
*				  protected           protected            no
*				  private             不可见               no
* private： 
*                 public              private              no
*                 protected           private              no
*				  private             不可见                no
*/
class A
{
public:
	int _pub_a;
protected:
	int _pro_a;
private:
	int _pri_a;//自己或者友元能访问，对子类也是不可见
};
//继承 public是访问限定符 A：基类/父类 B：子类/派生类
//继承过来后还附带了作用域，base:: _pub_a  --->所以在派生类中取一样的名字是没有问题的，因为两个变量作用域不同
class B:public A
{
public:
	int _pub_b;
protected:
	int _pro_b;
private:
	int _pri_b;
};
int main()
{
	return 0;
}
```
### 3.派生类的构造过程
```C++
#include<iostream>
using namespace std;
/*
派生类的构造过程
派生类怎么初始化从基类继承的成员变量：调用基类相应的构造函数
1.派生类可以从基类继承来成员变量和成员方法，除过构造函数和析构函数
2.派生类的构造函数和析构函数，负责初始化和清理派生类部分
派生类从基类继承的成员的初始化和清理 由基类的构造和析构函数负责
3.构造过程：
1.派生类调用基类的构造函数，初始化从基类继承过来的成员
2.调用派生类自己的构造函数，初始化派生类自己特有的成员
。。。。。派生类作用域到期后
3.调用派生类的析构函数，释放派生类成员可能占用的内存
4.调用基类的析构函数，释放派生类继承的基类成员可能占用的内存
*/


class base
{
public:
	base(int data) :ma(data) { cout << "base()" << endl; }
	~base() { cout << "~base" << endl; }
protected:
	int ma;
};
class derive :public base
{
public:
	derive(int data) :base(data), mb(data) { cout << "drive()" << endl; } //ma(data) X
	~derive() { cout << "~derive()" << endl; }
private:
	int mb;
};
int main()
{
	derive d(20);
	return 0;
}
```
### 4.多态共有继承
```c++

```
### 5.静态联编和动态联编
```c++

```
### 6.访问控制protected
```c++

```
### 7.抽象基类
```c++

```
### 8.继承和动态内存分配
```c++

```
### 8.重载 覆盖 隐藏
```C++
#include<iostream>
#include<typeinfo>
using namespace std;
/*虚函数 静态绑定 动态绑定
* 1.如果类里面定义了虚函数，那么编译阶段，编译器会给这个类类型产生唯一一个vftable虚函数表
* 虚函数表中主要存储的内容就是RTTI（run time type information：运行时类型信息）指针和虚函数的地址
* 当程序运行时，每一张虚函数表都会加载到内存的.rodata区（只读）
* 2.一个类里面定义了虚函数，那么这个类实例化的对象，运行时，内存中会存放一个vfptr（虚函数指针），指向相应类型的虚函数表vftable
* 一个类型定义的n个对象，其vfptr指向的都是同一张vftable
* 3.一个类里面虚函数的个数，不影响对象内存大小，（因为只加了一个vfptr），影响的是虚函数表（vftable）的大小
*/
class base //基类
{
public:
	base(int data =10):ma(data){}
	//虚函数
	virtual void show() { cout << "base::show()" << endl; }
	//虚函数
	virtual void show(int) { cout << "base::show(int)" << endl; }
protected:
	int ma;
};
class derive:public base
{
public:
	derive(int data =20):base(data),mb(data){}
	/*
	* 4.如果派生类中的方法和基类继承来的某个方法 返回值 函数名 参数列表都相同
	* 而且基类的方法是virtual函数，那么派生类的这个方法，自动处理成虚函数----覆盖？
	* 覆盖指的是虚函数表中虚函数地址的覆盖
	*/
	void show() { cout << "derive::show()" << endl; }//被自动处理成虚函数 
	//void show(int) { cout << "derive::show(int)" << endl; }
private:
	int mb;
};
int main()
{
	derive d(50);
	base* pb = &d;
	/*pb-> base类型 base::show 如果是普通函数，就进行静态绑定-> call base::show (0F71145h) 
	pb-> base base::show -->virtual 如果是虚函数就进行动态绑定,运行时才知道ecx寄存器里的地址
	*/
	pb->show(); //静态（编译时期）的绑定（函数的调用）call base::show (0F71145h)
	pb->show(10);// call base::show (0F71406h)
	cout << sizeof(base) << endl;
	cout << sizeof(derive) << endl;
	cout << typeid(pb).name() << endl;
	//pb--->base类型--->base有没有虚函数，如果没有，那么*pb识别的就是编译时期的类型*pb--base
	//如果有虚函数 那么*pb识别的就是运行时期的类型(RTTI类型)*pb--derive
	cout << typeid(*pb).name() << endl;
	return 0;
}
```
### 9.虚析构函数
```C++
/*
1.哪些函数不可以实现成虚函数
a 虚函数能产生函数地址，存储在vftable中，依赖于对象存在
b 对象---->vfptr--->vftable
c 构造函数不能是虚函数：其作用在对象构造之前 构造函数(全部都是静态绑定)中调用虚函数也不会发生动态绑定
d 析构函数可以 
e static 静态成员方法也不能作为虚函数
2.虚函数能用来干嘛，为什么会出现虚函数
3.虚析构函数
*/
#include<iostream>
#include<typeinfo>
using namespace std;
//什么时候必须把基类的析构函数实现成虚函数--基类的指针（引用）指向堆上的new出来的派生类对象的时候
//此时他调用析构函数的时候，必须发生动态绑定，否则派生类的析构函数无法调用

class base 
{
public:
	base(int data = 10) :ma(data) { cout << "base()" << endl; }
	virtual ~base() { cout << "~base()" << endl; }
	
	virtual void show() { cout << "base::show()" << endl; }
	
	//virtual void show(int) { cout << "base::show(int)" << endl; }
protected:
	int ma;
};
class derive :public base
{
public:
	derive(int data = 20) :base(data), mb(data) { cout << "derive()" << endl; }
	//基类的析构函数是虚函数，派生类的虚构函数自动成为虚函数
	~derive() { cout << "~derive()" << endl; }
	void show() { cout << "derive::show()" << endl; }
	
private:
	int mb;
};
int main()
{
	/*derive d;
	base* pb = &d;
	pb->show();*/
	base* pb = new derive(10);
	pb->show();
	//base()是虚函数，对于析构函数的调用就是动态绑定
	delete pb;//只会调用~base()
	return 0;
}
```
### 10.动态绑定
```C++
/*


*/
#include<iostream>
using namespace std;
class base
{
public:
	base(int data = 0) :ma(data) { cout << "base()" << endl; }
	virtual void show() { cout << "base::show()" << endl; }
	~base() { cout << "~base()" << endl; }
protected:
	int ma;
};
class derive:public base
{
public:
	derive(int data = 0) :base(), mb(data) { cout << "derive()" << endl; }
	void show() { cout << "derive::show()" << endl; }
	~derive() { cout << "~derive()" << endl; }
private:
	int mb;
};
int main()
{
	base b;
	derive d;
	b.show();//静态绑定
	d.show();//静态绑定
	//必须由指针（引用）调用虚函数
	base* pb = &b;
	pb->show();//动态绑定
	pb = &d;
	pb->show();//动态绑定
	base& rb1 = b;
	rb1.show();//动态绑定
	base& rb2 = d;
	rb2.show();//动态绑定
	return 0;
}
```
### 11.虚基类和虚继承
```C++
/*
1.
*/
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
/*

*/
#include<iostream>
using namespace std;
/*
* 1.多重继承-->代码复用--->一个派生类有多个基类
* a 虚基类：区别于抽象类（有纯虚函数的类）
* b virtual :修饰成员方法是虚函数 修饰继承方式是虚继承 被虚继承的类就称作虚基类
* c 内存布局：vbptr -- mb -- A::ma   vbptr--->vbtable 偏移量（vbptr+mb=8）
*/
class A
{
public:
	virtual void func() { cout << "call A::func()" << endl; }
	void operator delete(void* ptr)
	{
		cout << "operator delete p:" << ptr << endl;
		free(ptr);
	}
private:
	int ma;
};
class B :virtual public A
{
public:
	void func() { cout << "call B::func()" << endl; }
	void* operator new(size_t size)
	{
		void* p = malloc(size);
		cout << "operator new p:" << p << endl;
		return  p;
	}
private:
	int mb;
};

int main()
{
	//基类指针指向派生类对象，永远指向的是派生类的基类数据部分的起始地址，所以还可以访问func函数

    /*A* p = new B();
    cout << "main:p" << p << endl;
    p->func();*/
    //虚继承后派生类的基类数据部分挪到了后面，头部数据变为了vbptr,delete p释放不了从vbptr开始开辟的内存空间
    /*delete p;*/
    B b;
    A* p = &b;
    cout << "main:p" << p << endl;
    p->func();
	return 0;
}
```
### 4.菱形继承
```C++
//使用虚继承
```
### 5.类模板
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
#include<iostream>
using namespace std;
//C++语言级别提供的四种类型转换方式
/*
* 1.const_cast:去掉常量属性的类型转换
* 2.static_cast：提供编译器认为安全的类型转换(转换的类型之间要有联系)
* 3.reinterpret_cast：类似于c风格的强制类型转换
* 4.dynami_cast：主要用于继承结构中，可以支持RTTI类型识别的转换
*/

class base
{
public:
	virtual void func() = 0;
};
class derive1:public base
{
public:
	void func() { cout << "call derive1::func()"<< endl; }
};
class derive2:public base
{
public:
	void func() { cout << "call derive2::func()" << endl; }
	void derive2_func() { cout << "call derive2::derive2_func()" << endl; }
};
void showfunc(base* p)//识别传入的p的类型是哪个派生类 typeid(*p).name()?
{
	//dynamic_cast<>会检查指针是否指向的是一个derive2类型的对象
	//p->vfptr->vftable:RTTI 看类型是否为derive2
	//是 ：返回转换类型成功后derive2的地址，否则返回nullptr
	derive2* pd2 = dynamic_cast<derive2*>(p);//static_cast?这个是无论怎样都可以成功，全给转了，不能识别
	if (pd2 != nullptr)
	{
		pd2->derive2_func();
	}
	else
	{
		p->func();//动态绑定
	}
	
}
int main()
{
	const int a = 10;
	//int* p = (int *) & a;//----->char*p=(char*)&a  yes
	//const_cast<里面必须是指针或者引用类型>
	int* p = const_cast<int*>(&a);//类型要匹配 char*p=const_cast<char*>(&a) no
	//===========================================================================================
	//static_cast
	int b = 10;
	char c = static_cast<int>(a);
	int* p2 = nullptr;
	double* d = reinterpret_cast<double*>(p2);//不安全
	derive1 d1;
	derive2 d2;
	showfunc(&d1);
	showfunc(&d2);
	return 0;
}
```


