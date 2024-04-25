# 优先考虑 using 而非 typedef

## 结论

当编译器遇到类似 T::xxxx...这样的代码时 他不知道 xxxx 是一个 T 类型成员还是 T 数据成员 直到实例化才知道 但是为了处理模板 编译器必须直到表示的是 T 类型 默认情况下 C++假定通过作用域晕旋覆的名字不是类型
(补充 不仅 T::xxx 算 而且前面包裹了 T 就算 如 Myclass<T>::xxx 也是 也就是模板类型需要用 typename 强调 但是用 using 就能避免这个情况)

## 前置知识 函数指针

```cpp
int add(int a, int b){return a + b;}
int (*fp)(int, int) = add; // Ⅰ
int (*fp)(int, int) = &add; // Ⅱ

fp(1, 2); // Ⅲ
(*fp)(1, 2); // Ⅳ
```

由于函数名可以被降级为函数指针 以上 Ⅰ 和 Ⅱ 等效
而 Ⅲ 和 Ⅳ 的解引用可写可不写 效果一样

## using 和 typedef 的用法和区别

```cpp 用法
typedef long long LL;
using LL = long long;

LL var = 100;
```

```cpp 当用这两表示函数指针时
typedef int (*fp)(int, int);
using fp = int (*)(int, int);

struct Stu{ ... };
typedef void(Stu* fp)(int, double);
using fp = void(Stu*)(int, double);
typedef
```

看起来还是`using`更加明确指出 fp 才是别名 但这个理由也不是很能说服大伙都去用 using 吧 顶多是能看 不至于完全不用吧 别急 下面还有

## 前置知识 typename

定义:指定这个包含模板类型<T>的字符串是类型 而不是其他东西[包括变量和静态成员]
比如说最常见的`template<typename T>`中使用 typename 指定 T 就是个类型

另外的例子:

```cpp
template<typename T>
using MyVector = vector<T>;
```

另外一个例子 用 typename 消除歧义

```cpp
struct s1{
    int SubType;
}
```

### typename 使用例

Ⅰ 指定模板类时指定模板类型名称

```cpp
template<typename T>
```

Ⅱ 使用 typedef 时 再

```cpp
template<typename T>
struct Stu{
    typedef vector<T> MyVector;
};

template<typename T>
class MyClass{
    public:
    void func(){
        typename Stu::MyVector vec;
    }
};
```

## 前置知识 指向结构体/类 方法的指针

```cpp
struct Stu{
    int funcInStruct(int a, double b){
        return static_cast<int>(a +b);
    }
};

typedef int (Stu::*fp)(int, double);
fp ptr = &Stu::funcInStruct; // 这里一定要引用!!
Stu instance; // 记得要先实例化
cout << (instance.*ptr)(1, 2.f) << endl; // 这里一定要解引用
```

唉 就算指针可以省略&和\* 也得写 即方便看 也方便结构体/类的指针书写时不出错 using 也一个写法 故省略

## 用 using 来给结构体的指针作别名 alias

```cpp
struct Foo{
    void func(int a, double b){ ... };
};
using FooPtr = void(Foo*)(int, double);
Foo instance;
FooPtr ptr = &Foo::func;
instance.*ptr(1, 2.2);
```

## 只有 using 才能做到的操作

当涉及模板时 对于同样的内容 用 typedef 写会非常冗余 也难读

```cpp
// using
template<typename T>
using Myvector =  vector<T>; // √

// typedef的错误写法
template<typename T>
typedef vector<T> Myvector; // X 报错:

// typedef
template<typename T>
struct tmp{
    typedef vector<T> Myvector;
};
tmp<int>::Myvector vec; // 非常繁杂
```

看上面三个对比 就知道写起来就非常麻烦 而且还有一个更麻烦的点

```cpp
template <typename T>
struct vecClass{
    typedef vector<T> MyVector;
};

template<typedname T>
class testClass{
    public:
    void func(){
        // vecClass<int>::MyVector vec; // 光这么声明是不合格的
        typename vecClass<int>::MyVector vec; // 必须要用typename告诉编译器这坨是个类型 而不是别的什么东西
    }
};
```

基于结论写的那个原则 这里模板类::xxx 需要前面加一个 typename 来告诉编译器这确实是一个类型
而用`using`来写就是这样

```cpp
template <typename T>
using myVector_t = vector<T>;

template <typename T>
class testClass2
{
public:
    testClass2() = default;
    void func()
    {
        myVector_t<T> vec;
    }
};
```

简单明了还不会出错 `using`神中神吧

## 模板元编程中 怎么去掉 const &等修饰语 或令其编程左值引用

C++中有相关头文件 因为我自己也没写过什么大项目 没怎么用过这部分内容 就草草带过了

用法：`#include <type_traits>`
(不会 摆了)
