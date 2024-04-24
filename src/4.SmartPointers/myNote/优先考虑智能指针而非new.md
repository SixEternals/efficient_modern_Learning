## 条款二十一：优先考虑使用`std::make_unique`和`std::make_shared`，而非直接使用`new`

版本说明：
C11:仅有`std::make_shared`
C14:有`std::make_unique`和`std::make_shared`
如果你只是再用 C11 的话 别急 `std::make_unique`很好写的
（准备学完这本书之后开始学 miniSTL 学完再回来补）

```cpp
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
```

原理：
`make_unique`只是将它的参数*完美转发*到所要创建的对象的*构造函数*，从`new`产生的原始指针里面*构造出*`std::unique_ptr`，并返回这个`std::unique_ptr`。
