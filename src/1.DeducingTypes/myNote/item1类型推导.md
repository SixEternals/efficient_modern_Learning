# 类型推导 -- 理解模板类型推导

对于这段模板来说

```cpp
template<typename T>
void f(ParamType param);
```

对于这段模板来说 T 和 ParamType 推导不是一模一样的

## `ParamType`是一个指针或引用，但不是通用引用

保留 `const` 但是*忽略*引用`&`
example:

```cpp
template<typename T>
void f(T& param);               //param是一个引用
```

int x{1}; const int cx{x}; const int& crx{x};
f(x); // x 是 int T 被推导为 int param 被推导为 int &
f(cx); // x 是 int 推导 T 时保留 const 为 const int , param 推导为 const int&
f(crx); // 推导 T 时保留 const 去掉& 为 const int, param 推导为 const int &

## `ParamType`是通用引用

保留 const 而且会区分左右值

```cpp
template<typename T>
void f(T &&param);
```

## `ParamType`既不是指针也不是引用

传值 但是如果传入的是常量指针 那就会保持原样
`const char* const ptr` 则 T 推导还是`const char* const `

```cpp
template<typename T>
void f(T param);
```

## 数组实参

形如[]传入时会降级为指针`*` 即使本来[]和`*`不是同一坨东西

如果想要传入真正的数组[] 而不是指针的话 可以修改`f()`为传引用

```cpp
template<typename T>
void f(T &param);
```

除此之外 还有一种声明可以传入数组大小(对于原始数组来说)

```cpp
template<typename T, size_t N>
constexpr size_t getArraySize(T [&](N)) noexecpt
{
    return N;
}

// 使用
int arr[7];
constexpr auto size = getArraySize(arr); // 获取长度
int mappedVals[arraySize(keyVals)];    // 也可以这样使用
std::array<int, arraySize(keyVals)> mappedVals; // √
```

[constexpr 笔记](../../3.MovingToModernCpp/myNote/15_constexpr.md)

[noexecpt 笔记](../../3.MovingToModernCpp/myNote/14_noexcept.md)

看完上面两个笔记之后再解释一遍
在 c11 版本 函数体中只能有唯一一条返回语句 所以假设上述代码用的是 c11 那就不能写多余的语句也不能对元素进行修改 而在 c14 后放款了这一规定
arraySize 是一个固定的数值 也很适合设置为`constexpr` 存放在只读区域 提高了读取速度

而这个操作也是叶子操作 而且并没有涉及到资源的 alloc 不可能抛出错误 故使用`noexcept`

## 函数实参

同上 声明为

```cpp
void func(int, double);

template<typename T>
void f(T & param);

// 使用
f(func);
```

在函数里加一个引用即可 如果去掉那就和数组实参的结果一样 函数类型会退化为一个函数指针
