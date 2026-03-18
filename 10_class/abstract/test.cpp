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