
#include<iostream>
#include<typeinfo>
using namespace std;
int main(int argc,char*argv[])
{
	const int a =10;
	//我们希望常量的地址泄漏给普通指针，导致可以随意通过该指针解引用来改变该常量的值
	//int *p为普通指针，不能用于接收常量的地址	
	const int *p =(int *)&a;//const 修饰int 说明*p是常量，不能更改，也就是不可以通过解引用的方式改变a的值，但是指针可以改变指向的位置，也就是可以不指向a,但是无所谓，a保住了
	//int *const p const 修饰int* 也就是p为常量 说明指针的指向不能变，但可以通过解引用改变指向地址的值	
	cout << "a=" << a <<endl;
	cout << "*p=" << *p <<endl;
	cout << "p=" << p <<endl;
	return 0;
}
