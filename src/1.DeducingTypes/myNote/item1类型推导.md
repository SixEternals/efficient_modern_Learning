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
constexpr auto size = getArraySize(arr);
```

[constexpr 笔记](../../3.MovingToModernCpp/myNote/15_constexpr.md)

[noexpt 笔记]()

## 函数实参
