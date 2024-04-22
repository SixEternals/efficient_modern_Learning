## 2 `auto`类型推导

`auto`的推到规则遵循*条款 1 模板类型推导*的规则 但是有个别例外

唯一例外就是遇到`std::initializer_list<int>`

```cpp
auto x1 = 27; //x1为int，值为27
auto x2(27);//同上
auto x3 = {27};//x3为 std::initializer_list<int>,值为{27}
auto x4{27}; //同上

auto x5 = { 1, 2, 3.0 }; // error! 类型不一致，不能将推断为std::initializer_list<T>

```

### `std::initializer_list<int>`的解释

在编译器眼里这东西的优先级最高 几乎躲不了

#### auto

其对应的初始化表达式是一个花括号包围的列表时，auto 会推导出 std::initializer_list 类型的引用。

#### 模板参数推导中

如果模板参数是一个花括号包围的列表，编译器会尝试推导出列表中元素的类型，而不是 std::initializer_list。

```cpp
auto x = { 11, 23, 9 }; // x的类型为 std::initializer_list<int>

template<typename T> // 和auto x等同的模板类型推断
void f(T param);

f({ 11, 23, 9 }); // 错误！这里不能推断T的类型。
```

如果要达到 auto 的效果，得按照下面的方式来做：

```cpp
template<typename T>
void f(std::initializer_list<T> initList);
f({ 11, 23, 9 }); // T被推断为int, initList 的类型为 std::initializer_list<int>
```

因此`auto`类型推导和模板类型推导的真正区别在于，`auto`类型推导假定花括号表示`std::initializer_list`而模板类型推导不会这样（确切的说是不知道怎么办）。

## 不同版本区别

### 在 C++11

中使用 auto 时，这里比较容易出错，你本来想声明别的变量，最终却将其声明成了一个 std::initializer_list。因此，要谨慎使用统一初始化。

### 在 C++14

中，允许将 auto 作为函数返回值，也可以用其修饰 lambda 表达式中的参数。但是这些 auto 使用的都是模板类型推断，而不是 auto 类型推断，因此一个函数返回值为 auto 类型时，返回统一初始化式的值会出错：

```cpp
auto createInitList()
{
    return { 1, 2, 3 }; // 错误！不能推断{1，2，3}
}
// 下面的方式是对的：
std::initializer_list<int> createInitList()
{
    return { 1, 2, 3 }; //
}
```

## 总结

1. 模板类型推断是 auto 的基础，auto 关键字扮演了模板类型推断中的 T，而类型说明符扮演的是 ParamType。
2. 对于模板类型推断和 auto 类型推断，大多数场景下推断规则相通，有一种特殊情况，就是统一初始化式。
3. C14 中使用 auto 可以作为函数返回值，也可以作为 lambda 表达式的参数修饰符。**这里的 auto 使用的是模板类型推断，而不是 auto 类型推断。**

[参考链接](https://www.cnblogs.com/harlanc/p/10628321.html)
