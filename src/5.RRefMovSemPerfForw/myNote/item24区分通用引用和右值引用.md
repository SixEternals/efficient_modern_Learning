## 区分通用引用和右值引用

通用引用：

1. `auto&&`，因为`auto`采用*模板类型推导*而且有两个引号 -> 是通用引用
2. 如下 `template`和`T&&`紧挨着才算
3. 若满足第二点 但是被`const`修饰 那就不是通用引用了 be like

```cpp
template <typename T>
ReturnType func(const T && param); // 右值引用
```

```cpp
template<typename T>
ReturnType func(T && parma);

// 或者
template <typename T>
class vector{
public:
    ...
    template<class... Args>
    bool push_back(Args&&... value); // 算 紧挨着了
    ...
};
```

而下面这个不算

```cpp
template<class T, class Allocator = allocator<T>>   //来自C++标准
class vector{
public:
    ...
    void push_back(T&& value); // 这个不算 没有紧挨着
    ...
};
```

3. 在`lambda`表达式中 使用`auto&&`也算

```cpp
auto timeFuncInvocation =
    [](auto&& func, auto&&... params)           //C++14
    { // func是通用引用 params可以是0个或多个通用引用
        start timer;
        std::forward<decltype(func)>(func)(     //对params调用func
            std::forward<delctype(params)>(params)...
        );
        stop timer and record elapsed time;
    };
```
