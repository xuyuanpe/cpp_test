#include <iostream>
using namespace std;
class seqstack
{
public:
	seqstack(int size = 10)//构造函数
	{
		cout << this << "seqstack()" << endl;
		_pstack = new int[size];
		_top = -1;
		_size = size;
	}
	~seqstack()//析构函数
	{
		cout << this << "~seqstack()" << endl;
		delete[]_pstack;//释放数组空间要加[]
		_pstack = nullptr;
	}

	void push(int val)
	{
		if (full())
		{
			resize();
		}
		_pstack[++_top] = val;//先自增，再赋值
	}
	void pop()
	{
		if (empty())
		{
			return;
		}
		--_top;
	}
	int top() { return _pstack[_top]; }//返回栈顶
	bool empty() { return _top == -1; }//判空
	bool full() { return _top == _size - 1; }//判满
private:
	int* _pstack;//动态开辟数组，存储顺序栈的元素
	int _top;//指向栈顶元素的位置
	int _size;//数组扩容的总大小
	void resize()//类成员方法不希望外部调用
	{
		int* ptmp = new int[_size * 2];
		for (int i = 0; i < _size; ++i)
		{
			ptmp[i] = _pstack[i];
		}//memecpy(ptmp,_pstack,sizeof(int)*_size))/realloc都是内存拷贝---涉及到深拷贝和浅拷贝
		_pstack = ptmp;
		_size *= 2;
	}
}; int main()
{
	seqstack s;//调用构造函数
	//s.init(5);
	for (int i = 0; i < 15; ++i)
	{
		s.push(rand() % 100);

	}
	while (!s.empty())
	{
		cout << s.top() << " ";
		//cout << "" << endl;
		s.pop();
	}
	cout << "" << endl;
	//s.release();
	return 0;//依次进行析构
}