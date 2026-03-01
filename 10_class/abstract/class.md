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