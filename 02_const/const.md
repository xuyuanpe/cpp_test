# 引用和指针
##   const和一级指针的结合：
###  const和指针结合有三种情况：
#### 1.修饰指针指向的内容（值不能改）--->const int *p=&a
#### 2.修饰指针变量本身(指针指向不能改)---->int * const p
#### 3.同时修饰内容和指针本身(都不能改)----->const int *const p

## const和引用
## 顶层const和底层const:
#### 指针本身是一个对象，同时它又可以指向另一个对象，所以，指针本身是不是常量以及它指向的对象是不是常量是两个问题，我们要明白const修饰的是指针本身还是指针指向的对象
#### 顶层const----->表示的是指针本身是个常量
#### 底层const----->表示指针指向的对象是个常量
#### 指针类型既可以是顶层 const 也可以是底层const,这一点和其他类型相比区别明显
#### 例如：
```c++
            int i = 0；
            int *const p1 =&i; //顶层const，代表指针指向不能改变，但是指向对象的值可以改变
            const int ci = 20; //ci是个常量 不能将地址泄露给普通指针 
            const int* p2 =&ci;//底层const，表示指针指向的对象是个常量，不能改动，但是指针可以指向其他位置
            const int* const p3 = p2; //指针是个常量，指针指向的对象也是
            const int &r =ci; //用于声明引用的都是底层const
```

## 执行对象的拷贝操作时：
### 执行拷贝操作并不会改变被拷贝对象的值，所以拷入拷出对象是不是常量都没有关系
### 不能忽略底层const的限制，执行对象的拷贝操作时拷入和拷出的对象必须具有相同的底层const资格，或者两个对象的数据类型可以相互转换，也就是非常量可以转换为常量，反之则不行
```c++
      int *p= p3; // 错误：p3包含底层 const的定义，而p没有
      p2 = p3; // 正确：p2和p3都是底层const
      p2 = &i; // 正确：int*能转换成 const int*
      int &r = ci; // 错误：普通的int&不能绑定到int常量上
      const int &r2 =i; /1 正确：const int&可以绑定到一个普通int上
```
## const和二级指针的结合:

### 在此基础上，可以衍生出const和二级指针的结合
#### 1.二级指针的本质时是指向指针的指针
#### 2.const和二级指针的结合有三种情况

```c++
      const int **q;//---类型是int 修饰的是**q -->指的是对q的二次解引用的值不能被修改
      int* const *q;//---类型是int* 修饰的是*q --->指的是对q的解引用不能被修改
      int** const q; //---类型是int** 修饰的是q -->指的是q本身不能被修改
```
#### 3.const和多级指针结合的话，两边必须都有const

```c++
int main()
{
      const int a = 10;
      int *p1 = &a;  // 一级指针，指向a 这里已经错了
      int **p2 = &p1; // 二级指针，指向p1
      const int **p = &p1//这个是错误的，p的二次解引用虽然不可以改，但是可以通过修改p来间接修改a
      return 0;
}
```


## 1.const 限定符-----定义一个变量，它的值不能被改变
####   也就是防止程序不小心改变了这个值，所以有了const关键字
####   例如：const int buffersize =512；
####   const对象一旦创建后其值就不能被改变，所以在创建时就要初始化
####   而初始化的右值可以是任意复杂的表达式
## 2.在不改变const对象的操作中还有一种是初始化，利用一个对象区初始化另一个对象，是不是const都无所谓
## 3.默认状态下，const对象仅在文件内有效：
## 4.如果需要在其他文件使用，需要加上extern关键字
## 引用：一种更安全的指针，引用必须初始化，指针不一定要初始化，但是最好指向空指针
### 1.引用的使用：
```C++
//引用的定义
int a =10;
int &b =a;
int array[5]={};
int *p =array; 
int (&q)[]=array;
```
```C++
//1.左值引用和右值引用
void swap_by_pointer(int* a, int* b)
{
      int tmp =0;
      tmp =*a;
      *a =*b;
      *b =tmp;    
}
void swap_by_quoted(int &a,int &b)
{
      int tmp= 0;
      tmp =a;
      a=b;
      b=tmp;
}
int main()
{
      int a =10;
      int b =50;
      //int *p = &a;
      //int &b =a;
      //*p = 20;
      //cout << a << *p << b <<endl;
      //b =30;
      //cout << a << *p << b <<endl;
      //以上说明a *p b 都是在描述同一块内存
      cout << a << b <<endl;
      swap_by_pointer(&a,&b);
      cout << a << b <<endl;
      a=68;
      b=98;
      cout << a << b <<endl;
      swap_by_quoted(a,b);
      cout << a << b <<endl;
      return 0;
}
```
### 2.引用和指针的区别：
#### 引用和指针在汇编命令层面的命令是一样的
#### 引用只有一级引用
### 3.引用的优势


