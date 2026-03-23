#include<iostream>
//#include<string>
#include<vector>
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
	String(String &&str)
	{
		_pstr = str._pstr;
		str._pstr = nullptr;
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
	bool operator>(const String& other)const
	{
		return strcmp(_pstr, other._pstr) > 0;
	}
	bool operator<(const String& other)const
	{
		return strcmp(other._pstr, _pstr) > 0;
	}
	bool operator==(const String& other)const
	{
		return strcmp(other._pstr, _pstr) == 0;
	}
	int length()const { return strlen(_pstr); }//返回有效字符个数
	//char& str6.operator[](i)===>str6[i]===>char &
	char& operator[](int index) { return _pstr[index]; }//普通方法---可修改
	const char& operator[](int index)const { return _pstr[index]; }//常方法---仅读取
	//返回char*类型
	const char* c_str()const { return _pstr; }
	//
	

	//实现String的迭代器：提供一种统一的方式，来透明的遍历访问容器
	class iterator
	{
	public:
		iterator(char* p = nullptr) :_pit(p)
		{

		}
		bool operator!=(const iterator& it)
		{
			return _pit != it._pit;
		}
		void operator++()
		{
			++_pit;
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
String operator+(const String& lhs, const String& rhs)//没有delete过，会造成内存泄漏
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
	return tmp;//临时对象，调用拷贝构造函数

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
String GetString(String& str)
{
	const char* pstr = str.c_str();
	String tmpstr(pstr);
	return tmpstr;//局部对象想出去还得进行拷贝构造在main中产生临时对象
}
int main()
{
	/*String str1("aaaaaaaaaaaaaaaaaa");
	String str2;
	str2 = GetString(str1);
	cout << str2.c_str() << endl;*/
	cout << "-------------------------" << endl;
	String str1 = "aaa";
	vector<String> vec;//添加元素需要扩容
	vec.reserve(10);
	vec.push_back(str1);
	vec.push_back(String("bbb"));

		 

	return 0;
}