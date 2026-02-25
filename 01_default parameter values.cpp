/*
* 1.形参带有默认值的函数是什么样的 如：int func（int a =10，int b=20）
* 2.函数声明和定义时是不是都可以给形参赋默认值，能不能重复赋值，能不能只给其中一个赋值
* 3.这种形式有什么用，在性能优化方面
* 4.这种赋值有什么隐患，有什么注意事项
*/
#include <iostream>
using namespace std;
int sum(int a,int b)
{
	return a+b;
}
int main(int argc, char* argv[])
{
	int a =10;
	int b =20;
	cout << "test" << endl;
	return 0;
}
