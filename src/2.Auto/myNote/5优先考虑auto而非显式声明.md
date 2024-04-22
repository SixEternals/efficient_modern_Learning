## 优先考虑`auto`而非显式类型声明

前置知识：闭包就是 Lambda 表达式

### 好处 1：`auto`可以提示你初始化变量

```cpp
int x1;                         //潜在的未初始化的变量

auto x2;                        //错误！必须要初始化

auto x3 = 0;                    //没问题，x3已经定义了
```

### 好处 2：`auto`可以获取只有编译器才知道的类型

如果你写了一个闭包(lambda) 闭包的类型只有编译器才知道 我们可写不出来。这是就可以用上我们万能的`auto`了

```cpp
auto derefUPLess =
    [](const std::unique_ptr<Widget> &p1,       //用于std::unique_ptr
       const std::unique_ptr<Widget> &p2)       //指向的Widget类型的
    { return *p1 < *p2; };                      //比较函数
```

版本差异：
C11 只能对`lambda`的*返回值*使用`auto`
C14 不仅可以对`lambda`的返回值 还能对其*形参*使用`auto`

```cpp
auto derefLess =                                //C++14版本
    [](const auto& p1,                          //被任何像指针一样的东西
       const auto& p2)                          //指向的值的比较函数
    { return *p1 < *p2; };
```

### 好处 3：薄纱`std::function`

原书说：可能想使用`std::function` 而不是`auto`(谁问你了)

前置知识：`std::function`是什么
它*泛化*了函数指针的概念 ∴ 它不仅可以指向函数 也可以指向任何*能进行调用的*东西(包括`lambda`)

不过`std::function`最麻烦的一点就是 声明的时候要写出完整的函数签名
be like

假如有以下函数

```cpp
bool(const std::unique_ptr<Widget> &,           //C++11
     const std::unique_ptr<Widget> &)           //std::unique_ptr<Widget>
                                                //比较函数的签名
```

要用`std::function`对其进行绑定时 你就得这么写：（完整写出函数签名）

```cpp
std::function<bool(const std::unique_ptr<Widget> &,
                   const std::unique_ptr<Widget> &)> func;
```

因为`lambda`表达式能产生一个*可调用*对象 所以现在可以把这个闭包放到`std::function`里面
be like:

```cpp
std::function<bool(const std::unique_ptr<Widget> &,
                   const std::unique_ptr<Widget> &)>
derefUPLess = [](const std::unique_ptr<Widget> &p1,
                 const std::unique_ptr<Widget> &p2)
                { return *p1 < *p2; };
```

我擦 上面这个怎么看都很复杂吧

而且还有一个很致命的缺点！ 可能会引发*out-of-memory*的异常
这是因为 实例化`std::function`并声明一个对象 则该对象会有*固定*的大小
这个大小可能*不足以*存储一个闭包
此时`std::function`就会在堆上分配内存来存储 这就导致了它比`auto`更消耗内存 而且调用更慢 还可能会引发*out-of-memory*异常！(没空间的时候这个异常更加容易触发了)

#### `std::function`缺点小结

1. 冗余 需要写出完整的函数签名 即使是面对闭包时 也需要我们写出只有编译器才知道的类型
2. 空间问题 其实例化的对象大小固定 若超出会在堆上 new 空间 可能有堆溢出异常
3. 比`auto`慢

### 好处 4：避免类型写错了

最简单的一个坑就是`std::vector<int>::size_type`的返回值并不是`int`也不是`unsigned int`更不是`size_t`
而是大小会随操作系统位数而改变的*无符号整型*
`std::vector<int>::size_type`实际上被指定为无符号整型，所以很多人都认为用`unsigned`就足够了，写下了上述的代码。这会造成一些有趣的结果。举个例子：
在**Windows 32-bit**上`std::vector<int>::size_type`和`unsigned`是一样的大小
但是在**Windows 64-bit**上`std::vector<int>::size_type`是 64 位，`unsigned`是 32 位。这意味着这段代码在 Windows 32-bit 上正常工作
但是当把应用程序移植到 Windows 64-bit 上时就可能会出现一些问题。谁愿意花时间处理这些细枝末节的问题呢？

所以用`auto`保证不会出错

```cpp
auto sz =v.size();                      //sz的类型是std::vector<int>::size_type
```

再来一个例子：`std::unordered_map<Key, Value>`

```cpp
std::unordered_map<std::string, int> m;
…

for(const std::pair<std::string, int>& p : m)
{
    …                                   //用p做一些事
}
```

看起来很合理 实际上藏了一个很逆天的坑！
对于`std::unordered_map<Key ,Value>`中 _Key 是 const_!!!!
就是说上述语句中不可以写成
const std::pair<std::string, int>& p
而是要写出
const std::pair<**const** std::string, int>& p

这样写本来以为我们得到的 p 是 m 元素的引用 但是实际上是 m 元素的拷贝！亏了 xdm

使用`auto`就万无一失了

```cpp
for(const auto& p : m)
{
    …                                   //如之前一样
}
```

## 建议使用`auto`而并非强迫使用

这么说是因为`auto`还是有副作用的 滥用的话 肯定就 bug 满天飞了
问题是什么和怎么解决：

1. [初始化列表会产生意料不到的问题](../../1.DeducingTypes/myNote/item2auto类型推导.md)
2. [这个我还没写完 待会再补](../item6.md)

不过不用担心 闭包类型的问题已经有 IDE 缓解了一部分问题
现在鼠标悬浮在变量上 IDE 也能很智能的告诉你类型是什么 挺方便的

**请记住：**

- `auto`变量必须初始化，通常它可以避免一些移植性和效率性的问题，也使得重构更方便，还能让你少打几个字。
- 正如[Item2](../1.DeducingTypes/item2.md)和[6](../2.Auto/item6.md)讨论的，`auto`类型的变量可能会踩到一些陷阱。
