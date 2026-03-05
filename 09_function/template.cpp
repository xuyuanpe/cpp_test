#include <iostream>
#include <cstring>
using namespace std;
/*
函数模板：未实例化
模板的实例化----->在函数调用点，编译器用用户指定的数据类型，从原模板实例化一份函数代码
模板函数：模板实例化后生成的函数，编译器真正要编译的函数
模板类型参数：typename 或者 class定义
模板非类型参数:--->必须是整数类型或地址/引用都是常量，只能使用，不能修改
模板的实参推演：根据传入实参自行判断类型
模板的特例化（专用化）----不是编译器提供，而是开发者提供的
函数模板、模板的特例化、非模板函数的重载关系(不能说是重载，函数名不一样)

*/
//有模板，就可以只注重功能的实现，不用一直纠结类型----泛型
//template<class T>
template<typename T>//定义一个模板参数列表
bool compare(T a, T b)//compare 现在是一个函数模板，通过该模板可以创建出具体的函数
{
    cout << "call template compare" << endl;
    return a > b;
}
//针对compare函数模板，提供const char*类型的特例化版本
template<>
bool compare<const char*>(const char* a, const char* b)
{
    cout << "compare<const char*>" << endl;
    return strcmp(a, b) > 0;
}
//普通函数，非模板函数
bool compare(const char* a, const char* b)
{
    cout << "normal compare" << endl;
    return strcmp(a, b) > 0;
}
//模板的非类型参数---本身都是常量---只能使用而不能修改
template<typename T,int size>
void sort(T *arr)
{
    for(int i =0;i<size -1;++i)
    {
        for(int j =0;j<size -1-i;++j)
        {
            if(arr[j]>arr[j+1])
            {
                int tmp =arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=tmp;
            }
        }
    }
}

int main()
{
    //模板名<类型名>(参数1，参数2)；====>具体的函数
    compare<int>(10, 20);
    //在函数调用点，编译器用用户指定的数据类型，从原模板实例化一份函数代码
    /*
    * bool compare<int>(int a, int b)
    {
        return a>b;
     }
    */
    compare<double>(10.0, 25.5);//一个类型的函数只生成一次，下次再有同类型的需求会调用之前生成的函数
    compare(20, 30);//不给具体的数据类型，编译器可以根据传入的实参类型来自己推断参数类型===>模板实参推演
    //compare(20, 30.5)；模板只给出了一个类型，而传入了两个类型，编译器会报错
    //特例化版本
    compare("aaa", "bbb");//编译器优先将其看做函数名，调用普通函数，而不是特例化
    compare<const char*>("aaa", "bbb");//视作模板特例化
    int array[]={12,5,7,89,32,21,35};
    const int size =sizeof(array)/sizeof(array[0]);
    sort<int,size>(array);
    for(int val:array)
    {
        cout << val <<" ";
    }
    cout << endl;
    return 0;
}
//注意，在多文件编程中，模板本身是不编译的，所以模板不能在一个文件定义，另一个文件中使用
//模板代码调用之前，一定要看到模板定义的地方，这一样模板才能产生能被编译器编译的代码
//所以模板代码一般都是放在头文件的，然后让源文件直接包含 类似的还有stl模板
//或者告诉编译器进行指定类型模板的实例化（告知具体类型）