//string类型：编写类string的构造函数 析构函数 赋值函数
//循环队列queue
#define _CRT_SECURE_NO_WARNINGS //禁用安全警告
#include <iostream>
#include <cstring>
using namespace std;
class queue
{
public:
    //构造函数
    queue(int size = 10)
    {
        _pqueue = new int[size];
        _front = _rear = 0;
        _size = size;
    }
    //delete
    //queue(const queue& other)=delete;
    //queue& operator=(const queue&) =delete; //这样使用拷贝构造函数和赋值重载函数就会报错
    //拷贝构造函数
    queue(const queue& other)
    {
        
        _front =other._front;
        _rear =other._rear;
        _size =other._size;
        _pqueue =new int [_size];
        for(int i =_front;i!=_rear;i=(i+1)%_size)
        {
            _pqueue[i]=other._pqueue[i];
        }
    }
    //赋值重载函数
    queue& operator=(const queue & other)
    {
        if(this == &other)
        return *this;
        delete[]_pqueue;
        _front =other._front;
        _rear =other._rear;
        _size =other._size;
        _pqueue =new int [_size];
        for(int i =_front;i!=_rear;i=(i+1)%_size)
        {
            _pqueue[i]=other._pqueue[i];
        }
        return *this;
    }
    //析构函数
    ~queue(void)
    {
        delete[]_pqueue;
        _pqueue = nullptr;
    }
    void push(int val)//队尾进、队头出
    {
        if (full())
        {
            resize();
        }
        else
        {
            _pqueue[_rear] = val;
            _rear = (_rear + 1) % _size;//循环队列
        }
    }
    void pop()//出队
    {
        if (empty())
        {
            return;
        }
        else
        {
            _front = (_front + 1) % _size;

        }
    }
    int front()//获取队头元素
    {
        return _pqueue[_front];
    }
    bool full()
    {
        return (_rear + 1) % _size == _front;
    }
    bool empty()
    {
        return _front == _rear;
    }
private:
    int* _pqueue;//动态开辟数组空间
    int _front;//队头
    int _rear;//队尾
    int _size; //队列大小
    void resize()//扩容需要把队列中的元素一个个挪过去，然后front在队头
    {
        int* ptmp = new int[2 * _size];
        int index = 0;
        for (int i = _front; i != _rear; i = (i + 1) % _size)
        {
            ptmp[index++] = _pqueue[i];
        }
        delete[]_pqueue;
        _pqueue = ptmp;
        _front = 0;
        _rear = index;
    }
};
class String
{
public:
    String(const char* str = nullptr)//普通构造函数
    {
        // if(str!=nullptr)//这样可行，但是之后每一次都要判断为不为空
        // {
        //     m_data =new char[strlen(str)+1];
        //     strcpy_s_s(this->m_data,str);
        // }
        // else
        // {
        //     m_data=nullptr;
        // }  
        if (str != nullptr)
        {
            m_data = new char[strlen(str) + 1];
            strcpy(this->m_data, str);
        }
        else
        {
            m_data = new char[1];
            *m_data = '\0';//这样可以避免每一次都要对是否为空进行判断
        }


    }
    String(const String& other)//拷贝构造函数
    {
        m_data = new char[strlen(other.m_data) + 1];
        strcpy(m_data, other.m_data);


    }
    ~String(void)//析构函数
    {
        delete[]m_data;
        m_data = nullptr;
    }
    String& operator=(const String& other)//赋值重载函数---返回*this 可以支持连续赋值，所以不使用void
    {
        if (this == &other)//防止自赋值
        {
            return *this;
        }
        delete[]m_data;//释放当前堆内存
        m_data = new char[strlen(other.m_data) + 1];
        strcpy(m_data, other.m_data);
        return *this;
    }
private:
    char* m_data;//用于保存字符串
};
int main()
{
    //调用带const char *参数的构造函数
    String s;
    String s1("hello");
    String s2 = "hello";
    //调用拷贝构造函数
    String s3 = s2;
    String s4(s2);
    //调用赋值重载函数---能否支持连续赋值
    s1 = s2;

    queue q(20);
    for (int i = 0; i < 20; ++i)
    {
        q.push(rand() % 100);
    }
    while (!q.empty())
    {
        cout << q.front() << " ";
        q.pop();
    }
    cout << endl;
    queue q2 =q;
    q2 =q;
    return 0;
}