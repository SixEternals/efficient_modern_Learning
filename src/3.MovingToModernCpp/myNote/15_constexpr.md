# 尽可能用`constexpr`

## 特点

1. constexpr 创建了一个编译时常量; const 仅仅意味着值不能被改变。
2. 编译器已知的值“享有特权” 他们可能被存到*只读*存储空间中（这个特性再嵌入式中很有用）
3. vector array...等等容器需要传入容器长度时 需要传入 constexpr
4. std::pow()返回的并非 constexpr 值 基于第三点 所以不能写`vector<int , pow(3, n)> res` 除非自己重新写一个自己的 pow()函数 并使用`constexpr`标记

```cpp
// 而在C++14和C ++17中并非如此。在C++11下声明为
constexpr void f();

// needs to be declared as 需要声明为
constexpr void f() const;
```

## 不同 C 版本中`constexpr`的区别

C11 中：

1. 函数体内只允许有一个 return 语句(但是可以用三元运算符和递归来把语句压缩到 return 这行 [跳转 36 行 自己看](../item15.md))
2. 而且不可以对 void（非字面值类型） 函数使用`constexpr`
3. 而且被`constexpr`标记的函数里不可以修改元素

以上在 C14 版本以上就放宽了

## `const`和`constexpr`的区别

对于**变量**
前者可以在编译期或运行期间赋值，后者*只能*在编译期间赋值

This is an error: 这是一个错误：

```cpp
int i =1;
const int j=i; // j is runtime const
constexpr int k=j; // Error
```

However, this is successfully compiled with GCC 10.2:
但是，这在 GCC 10.2 中编译成功：

```cpp
const int j=1; // set at compile-time
constexpr int k=j;
```

所以多用**constexpr**

### 用处 1：编译时常量

```cpp
constexpr int MinValue = 1;
constexpr double Pi = 3.14159;
```

### 用处 2：针对函数，可得到在编译期间/运行期都可运行的函数

```cpp
constexpr fib(constexpr a){

}
```

### 用处 3：`constexpr`标记构造函数运行在编译器创建对象

前提！！！：构造函数对象所有参数都是`constexpr`表达式

```cpp
class ConstRxprTest
{
public:
    constexpr ConstRxprTest(int v) : val(v) {}

private:
    int val;
};
```

### 用处 4：if constexpr

C++17 引入`if constexpr` 它允许在编译时根据条件选择分支

```cpp
template<bool flag, typename T>
void myFunction(T param) {
    if constexpr (flag) {
        // 这段代码仅在 flag 为 true 时编译
        std::cout << "Flag is true, doing something with " << param << '\n';
    } else {
        // 这段代码仅在 flag 为 false 时编译
        std::cout << "Flag is false, doing something else with " << param << '\n';
    }
}

int main() {
    myFunction<true>(10);  // 仅会编译并运行 if constexpr 中的代码
    myFunction<false>(20); // 仅会编译并运行 else 中的代码
}
```

> ======================================

### 用处 5：多线程

它们在创建任何线程之前就被初始化了。故它们的访问永远不需要任何同步。
意味着你不需要使用任何形式的*同步机制*（如互斥锁）来保护对这些对象的访问

### 用处 6：constexpr 在函数中的用处

首先要知道它在标记函数之后有什么特性：

1. 若实参全是编译器常量 则函数将产出编译期常量
2. 若实参里包含运行期值时 则它会像普通函数一样运算 返回运行期值
   也就是说不需要准备两个重载函数 一个重载接收编译期参数 另外一个重载接受运行期参数 `constexpr`来了全做了 一把子精简代码了

### 用处 7：结合元编程 来写重载函数

(我天 又是元神 好难)

[参考链接](https://iamsorush.com/posts/cpp-constexpr/#case-study-3)

\*!!!报错！！！无法重载仅按\*\*

```cpp
int g(int i)
{
    return 10;
}
std::string g(int i)
{
    return "hello";
}
```

使用元编程和`constexpr`相结合来解决这问题

```cpp
template <int i>
auto f()
{
    if constexpr (i == 0)
    {
        return 10;
    }
    else if constexpr (i == 1)
    {
        return "nihao";
    }
}
int main(int argc, char const *argv[])
{ // 0 1都是右值 所以也算编译期值了
    cout << f<0>() << endl;
    cout << f<1>() << endl;
    return 0;
}
```

Note that while at compile-time they are the same, but at runtime, f<0> and f<1> are two different animals with different signatures.
请注意，在编译时，它们是相同的，但在运行时， f<0> 和 f<1> 是两个不同的动物，具有不同的签名。

### 用处 8：避免重载 2

这个我感觉平常用不上 还是等工作时遇到再继续研究吧 详情请看链接
[参考链接](https://iamsorush.com/posts/cpp-constexpr/#case-study-3)

## 小结

本条款建议尽量使用`constexpr`
学完上面的就知道了 被`constexpr`修饰的变量和函数 的适用范围会比没被修饰的大得多
其好处就是通过在编译器中进行一些简单的运行 并将值存储在只读的区域 从而减少运行时间来优化(小缺点就是编译时间变长了 我寻思这个副作用也不差吧)
