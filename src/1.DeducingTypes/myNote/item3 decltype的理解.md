## 理解`decltype`

给`decltype`一个变量名或表达式 它就会*精准*的告诉你结果(除了某些情况 ，在本文末尾介绍) ，而且它和`auto`和模板类型推导的规则不完全一样

### `auto`和`decltype`比较

decltype:

```cpp
const int i = 0;                //decltype(i)是const int 顶层const被保留

bool f(const Widget& w);        //decltype(w)是const Widget&
                                //decltype(f)是bool(const Widget&)

struct Point{
    int x,y;                    //decltype(Point::x)是int
};                              //decltype(Point::y)是int

Widget w;                       //decltype(w)是Widget

if (f(w))…                      //decltype(f(w))是bool

template<typename T>            //std::vector的简化版本
class vector{
public:
    …
    T& operator[](std::size_t index);
    …
};

vector<int> v;                  //decltype(v)是vector<int>
…
if (v[0] == 0)…                 //decltype(v[0])是int&

int arr[] = {1, 2, 3};
decltype(arr) arr_copy = arr;  // arr_copy的类型被推导为int[3]，即使arr没有被赋予一个变量
```

auto:

```cpp
int& ref = x;
auto ref_copy = ref;  // ref_copy的类型被推导为int，而不是int&

const int cx;
auto acx = cx; // 推导为int 顶层const被丢弃
```

最主要的区别就在于
`auto`推导时不保留引用 也不会保留 const ，
而`decltype`来了全保留了 ，确实很精准

### `decltype`最主要的用途就是用于声明函数模板，该函数的 ReturnType 依赖于形参类型

### `decltype`的稀少情况

一般来说 它可以*完美推断*出真正的类型
但是比单纯的变量名*更复杂*的左值表达式 它总是返回*左值引用*

即

```cpp
int x{0};

decltype(x) y; // y是int
decltype((x)) z; // z是int&
```

需要额外说明的一点是 在 c++中 多余的括号是可以接受的
be like

```cpp
int x{0}; //int
int (x){0}; // int
int ((x)){0}; // int
```

所以对于上面的情况 本以为推导的是 x 的`int`类型 但是却返回了`int&`
因为编译器以为加了括号就是复杂的左值表达式
∴

```cpp
    decltype((x)) x1{0}; // 报错！ 这是左值引用 不能用右值初始化
    decltype((x)) x2{x}; // √
```

看起来无伤大雅？当到 C14 允许用`decltype(auto)`作为返回类型时 那就要出大问题了

```cpp
decltype(auto) f1()
{
    int x = 0;
    …
    return x;                            //decltype(x）是int，所以f1返回int
}

decltype(auto) f2()
{
    int x = 0;
    return (x);                          //decltype((x))是int&，所以f2返回int&
}
```

注意不仅`f2`的返回类型不同于`f1`，而且它还**引用了一个局部变量**！这样的代码将会把你送上未定义行为的特快列车，一辆你绝对不想上第二次的车。

顺带一提在 c11 中不可能有这种问题 因为函数的局部变量不能出现在后置返回类型里

```cpp
template <typename T>
auto testfunc1()
    -> decltype(x) // 报错！ x未定义
{
    int x{0};
    return x;
}
template <typename T>
auto testfunc2()
    -> decltype(x) // 报错！ x未定义
{
    int x{0};
    return (x);
}
```

### 如何检查 decltype 的推到结果是不是想要的？

看第四章

**请记住：**

- `decltype`总是不加修改的产生变量或者表达式的类型。
- 对于`T`类型的不是单纯的变量名的左值表达式，`decltype`总是产出`T`的引用即`T&`。
- C++14 支持`decltype(auto)`，就像`auto`一样，推导出类型，但是它使用`decltype`的规则进行推导。
