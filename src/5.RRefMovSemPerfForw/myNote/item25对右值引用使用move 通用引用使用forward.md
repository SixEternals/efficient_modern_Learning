## 对右值引用使用`std::move`，对通用引用使用`std::forward`

### 原因

总而言之，当把右值引用转发给其他函数时，右值引用应该被**无条件转换**为右值（通过`std::move`），因为它们**总是**绑定到右值
当转发通用引用时，通用引用应该**有条件地转换**为右值（通过`std::forward`），因为它们只是**有时**绑定到右值。

反过来想 如果给万能引用使用`std::move`会发生什么？

```cpp
class Widget {  // 一个通用引用例子
public:
    template<typename T>
    void setName(T&& newName)       // 通用引用可以编译，
    { name = std::move(newName); }  // 但是代码太太太差了！
    …
private:
    std::string name;
    std::shared_ptr<SomeDataStructure> p;
};

// 调用
std::string getWidgetName();        //工厂函数
Widget w;
auto n = getWidgetName();           // n是一个局部变量
w.setName(n);                       // 把n移动进w！ 那么n保留的资源就没了 和野指针差不多概念
…                                   // 现在n的值未知
```

你看 n 是个左值 它进去后被`move`了 在第 24 条款知道 `std::move`使用了`static_cast`来强制转换
那就是说 如果我们传入的是右值 万能引用帮我们把该右值绑定到右值引用上 然后遇上`move`也没事 本来就是临时值 被移走就移走吧
但是如果传入左值 万能引用帮我们把该左值绑定到左值引用上 然后遇上`move`后资源被移走了！ 那我函数调用完之后我的`n`那咋办 里面的值都未知了

#### 用重载优化吗？

可以 但是缺点很明显 ：

1. 编写和维护的代码增多 假如有 n 个类 那就至少有`2^n`个重载函数需要重载
2. 效率下降：在某些情况下，使用重载可能导致不必要的性能开销。例如，当使用字面量字符串初始化 Widget 对象的 name 成员时：

```cpp
class Widget {
public:
    void setName(const std::string& newName)    // 用const左值设置
    { name = newName; }

    void setName(std::string&& newName)         // 用右值设置
    { name = std::move(newName); }
    …
};
// 调用
Widget w;
w.setName("Adela Novak");
```

如果使用通用引用（即模板），字面量字符串可以直接赋值给 w.name，而无需创建临时的 std::string 对象。
然而，由于有两个重载，编译器*可能*会生成*一个临时*的 std::string 对象来绑定字面量字符串，然后通过移动赋值将这个临时对象的内容移动到 w.name。

##### 性能开销：

1. 构造函数：创建一个临时 std::string 对象。
2. 赋值运算符：将临时对象移动到 w.name。
3. 析构函数：销毁临时对象。

这种额外的开销可能在性能敏感的应用中变得重要，尤其是当涉及大量对象的赋值操作时。

##### 泛化情况：

如果 Widget 的成员是任意类型，而不仅仅是 std::string，性能差距可能会更显著。因为并非所有类型的移动操作都像 std::string 那样高效。

##### 重载最重要的问题是*可拓展性差*

但是，关于对左值和右值的重载函数最重要的问题不是源代码的数量，也不是代码的运行时性能。而是设计的*可扩展性差*

函数的典型代表是`std::make_shared`，还有对于 C++14 的`std::make_unique`

```cpp
template<class T, class... Args>                //来自C++11标准
shared_ptr<T> make_shared(Args&&... args);

template<class T, class... Args>                //来自C++14标准
unique_ptr<T> make_unique(Args&&... args);
```

这里只能用万能引用搭配参数包的写法 来接收多个指针
什么 你问我能不能用左右值重载再分别使用参数包？ 答案是不行！
详情看链接 我拿出一部分来讲解

一开始我困惑的点在于能不能写出：

```cpp
template<class T, class... Args>
shared_ptr<T> make_shared(const Args&... args); // 左值引用重载版本

template<class T, class... Args> // 我寻思这不还是万能引用吗... 加不加const都是万能引用 那么就会和左值引用版本冲突了 导致编译器报错或者乱选
shared_ptr<T> make_shared(const Args&&... args); // 万能引用的只读版本
```

然后看完链接之后发现 就拿上面左值引用重载版本举例子吧

```cpp
template<typename T,typename... Args>
void foo(const T &t, const Args& ... reset) // 左值引用重载版本
{
    //...
}

// 调用
int main()
{
    int i = 0;
    double d = 3.14;
    string s = "how now brown cow";

    foo(i, s, 42, d); //包含三个参数
    foo(s, 42, "hi"); //包含二个参数
    foo(d, s);        //包含一个参数
    foo("hi");        //空包

    return 0;
}
```

！！重点来了 上面的调用在编译器眼里就是
上面的四个 foo()函数调用会实例化下面 4 个版本：

```cpp
//第一个T的类型从第一个实参推断出来，剩余的实参从提供的额外实参中推断出
void foo(const int&, const string&, const int&, const double&);
void foo(const string&, const int&, const char[3]&);
void foo(const double&, const string&);
void foo(const char[3]&);
```

由此可见 即使我们传入`42`这种字面量值(右值) 传进去被推到成了*左值引用*

而回到最开始的疑惑：能不能用左右值重载再分别使用参数包？
可以 但是需要看函数功能本身 ：

1. 如果严格需要对传入参数进行正确的转发`std::forward` 或得到参数们本身的类型 而不左值绑定右值引用 右值绑定左值引用这样 那就需要学习`std::unique_ptr`的写法了——使用万能引用
2. 如果没有要求 而且经过考虑之后发现不影响后续开发的话 那就用吧 比如本身就保证了传入一系列参数*都是左值*的话 那确实可以**只**写左值引用版本`void foo(const Args& ... vars)`

`std::unique_ptr`和`std::shared_ptr`的本意是必须能够处理任意数量 任意类型的参数
就是说它设计的本意是 需要能一次性处理左值 右值 而不是把他们全部都用左值引用绑定 那本来也违背了左值不能绑定右值等等的条例 所以原书才说“只能使用万能引用了 只有这一条路可走”

如果使用万能引用的话 be like

```cpp
template<typename... Args>
void foo(const Args&& ... reset); // 万能引用 只读

// 调用
int main()
{
    int i = 0;
    double d = 3.14;
    string s = "how now brown cow";

    foo(i, s, 42, d); //包含三个参数
    foo(s, 42, "hi"); //包含二个参数
    foo(d, s);        //包含一个参数
    foo("hi");        //空包
    return 0;
}

// 展开为
void foo(const int&, const string&, const int&&, const double&);
void foo(const string&, const int&&, const char[3]&);
void foo(const double&, const string&);
void foo(const char[3]&);
```

如上 会自动判别每个参数的左右值类型了

[必看外链 参数包(... )的原理](https://blog.csdn.net/qq_41453285/article/details/104505789)

### 如果你在**按值**返回的函数中，返回值绑定到右值引用或者通用引用上，需要对返回的引用使用`std::move`或者`std::forward`。

原因：

1. 移动语义
   不拷贝 提高性能

2. 返回值优化（RVO）和命名返回值优化（NRVO）
   现代编译器通常实现了返回值优化（RVO）或命名返回值优化（NRVO），这些优化减少了按值返回时的临时对象创建。然而，即使在 RVO/NRVO 存在的情况下，如果返回的对象是通过右值引用或通用引用创建的，使用 std::move 或 std::forward 仍然可以确保对象是通过移动而非拷贝来构造的。

#### 右值引用情况

例子：

```cpp
Matrix类内：                              //按值返回
operator+(Matrix&& lhs, const Matrix& rhs)
{
    lhs += rhs;
    return std::move(lhs);	        //移动lhs到返回值中
}
```

如果不用`std::move` 就触发拷贝了。

1. 加入自定义的`Matrix`支持移动操作，而且比拷贝效率更高，则在`return`语句中使用`std::move`效率也高。

2. 如果`Matrix`不支持移动操作，则将它转换为右值也可以，因为 const 右值可以被拷贝构造函数接受(很违反常理？还真是，不过它真的是合法的，出自[为 lvalue-reference-to-`const`允许被绑定到一个`const`右值上,最后几段](../item23.md))。

综上 如果**返回一个按值传递的右值/通用引用，则对返回的引用使用`move` `forward`** , 因为如果不支持移动操作 也有拷贝构造接受，如果后续添加一个高效的移动函数 那就更加受益了 百益而无一害

#### 通用引用情况

```cpp
template<typename T>
Fraction                            //按值返回
reduceAndCopy(T&& frac)             //通用引用的形参
{
    frac.reduce();
    return std::forward<T>(frac);		//移动右值，或拷贝左值到返回值中
}
```

1. 假如 frac 是右值 则 std::forward<T>(frac)在这里用于转发 frac。转发时，如果 frac 是一个右值，它将保持为右值引用，从而允许移动构造函数的使用

2. 加入 frac 是左值 它将保持为左值引用，从而调用拷贝构造函数

### 典中典错误值 对函数局部变量使用`std::move`

阅读完上面的部分 可能会有些人觉得：“既然可以对 const 右值使用`std::move` 然后就可以调用移动操作了，那我可不可以把函数内的局部变量也给`移动`出去 来提高性能？”，他们操作如下

```cpp
// 原始代码
Widget makeWidget()                 //makeWidget的“拷贝”版本
{
    Widget w;                       //局部对象
    …                               //配置w
    return w;                       //“拷贝”w到返回值中
}
```

他们想要“优化”代码，把“拷贝”变为移动：

```cpp
// 想把局部变量"移动出去" 以此做到优化
Widget makeWidget()                 //makeWidget的移动版本
{
    Widget w;
    …
    return std::move(w);            //移动w到返回值中（不要这样做！）
}
```

可这么想是错的，我们伟大的编译器是可以通过**返回值优化** 来将上述的`w`直接分配给函数返回值的内存中直接构造对象。即*省去*一次拷贝

## `RVO`和`NRVO`的介绍

`RVO`:return value optimization
`NRVO`:Named return value optimization
其中`NRVO`为`RVO`的特殊情况

这些优化的目的是减少或消除在函数返回时创建临时对象的需要。而且他们都是编译器层面的优化 并非语言特性

我个人觉得知道有这个优化就行了，懒得深究这两的工作原理，只要知道它们的作用即可，这种优化警告程序员不要随便乱用`std::move`，如果你给可以被`RVO`或`NRVO`的局部变量使用`std::move`的话 会适得其反 可能还影响了编译器自己把他们优化的行为

## 总结：

1. 如果返回值绑定到右值引用/通用引用 对返回值使用`std::move`或`std::forward`
   be like

   ```cpp
    template<typename T>
    T&& func(T&& lhs, T&& rhs){ // 芝士通用引用版本，右值引用照葫芦画瓢就可
        lhs += rhs;
        return std::move(lhs);
    }
   ```

   而且要注意是在**最后一次**使用时 在对右值引用上使用`std::move`，在通用引用上使用`std::forward`。

2. 对按值返回的函数要返回的右值引用和通用引用，执行相同的操作。

3. 如果局部对象可以被*返回值优化*消除，就绝不使用`std::move`或者`std::forward`。
