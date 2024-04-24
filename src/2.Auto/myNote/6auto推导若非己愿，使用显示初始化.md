## 若`auto`推导若非己愿 使用显式类型初始化

在[item5](./优先考虑auto而非显式声明.md) 中我们见识到了`auto`的强大和简洁 但是还是有例外情况

### `std::vector<bool>`的与众不同

对于大多数 T `std::vector<T>`返回的都是 T 的引用（T&）
而只有`T == bool`时是个例外

`std::vector<bool>`是为了节省空间而特殊设计的，它使用*一个比特位*来存储布尔值，_而不是通常的一个字节_。由于 C++*不允许*对单个比特位进行引用，std::vector<bool>的 operator[]不能返回一个 bool&。故，返回一个代理对象 `std::vector<bool>::reference`
这个代理对象允许程序员通过 std::vector<bool>对象对单个比特位进行操作，同时保持了紧凑的存储格式。

代理对象 `std::vector<bool>::reference` 重载了 赋值操作符和 转换操作符 (在后面解释)，使得它的行为类似于 bool&。然而，它实际上并*不是*一个 bool&。
这种设计允许程序员像使用普通引用一样使用这个代理对象，同时避免了直接对单个比特位进行引用所带来的问题。

总结来说：
std::vector<bool>的 operator[]不返回 bool&的原因是为了维持 std::vector<bool>的紧凑存储特性，同时提供一个安全的接口来操作单个比特位。
代理对象 std::vector<bool>::reference 提供了这样的接口，但它并不是对布尔值的直接引用。

#### 解释 “重载了赋值操作符和转换操作符”

1. 赋值操作符重载
   代理对象 std::vector<bool>::reference 重载了赋值操作符（operator=），使得它可以像一个普通的布尔引用一样被赋值。例如：

```cpp
std::vector<bool> v(10);
v[0] = true; // 通过代理对象赋值
```

v[0]返回一个 std::vector<bool>::reference 类型的代理对象。通过这个代理对象，我们可以给 v 的第一个元素赋值为 true。

2. 转换操作符重载
   std::vector<bool>::reference 还重载了转换操作符（通常是 operator bool()），允许代理对象在需要布尔值的地方使用，而不需要显式地进行类型转换。例如：

```cpp
if (v[0]) {
// 执行某些操作
}
```

v[0]同样返回一个代理对象。由于代理对象重载了转换操作符，它可以直接在布尔表达式中使用，自动转换为布尔值。

[参考链接 为什么 vector<bool>不会返回 bool&](https://stackoverflow.com/questions/8399417/why-vectorboolreference-doesnt-return-reference-to-bool)

#### 回到`auto`的话题

```cpp
std::vector<bool> features(const Widget& w);

bool highPriority = features(w)[5];     // ① bool
auto highPriority = features(w)[5];     // ② std::vector<bool>::reference

// 对以上这两种写法分别调用↓
processWidget(w,highPriority);
```

```cpp
bool highPriority = features(w)[5];     // ① bool
processWidget(w,highPriority); // 可以 正确调用
// 因为代理类std::vector<bool>::reference可以隐式转换为`bool`了
```

```cpp
auto highPriority = features(w)[5];     // ① bool
processWidget(w,highPriority); // 居然出现未定义行为！？
```

看到上面对`std::vector<bool>`的解释后 就突然释怀了 因为`auto`推导出来的不是`std::vector<bool>::reference`而不是`bool&`
由于返回的不是`bool&`(因为本来就没有这个东西) 而是给你返回一个`std::vector<bool>`的*临时对象*，因为没有名字 我们就叫他 tmp 吧。
此时`highPriority`的属性是`std::vector<bool>::reference` 但是它是从`features(w)[5]`中获得一个**临时值**
这个临时值过了`auto highPriority = features(w)[5];`这一行之后就被销毁了(临时值的特性哦 别忘了) 那么此时代理类就置空了 那此时传进去`processWidget`确实会出现未定义行为

- 未定义行为的原因
  当 temp【features(w)的返回值，一个未命名的 std::vector<bool>对象】被销毁时，highPriority 中的*代理对象*仍然持有对 temp 的引用。
  这导致在后续使用 highPriority 时，如 processWidget(w, highPriority);，出现未定义行为，因为代理对象尝试访问*已经销毁*的对象。

#### 引申

```cpp
vector <bool> v;
bool *pb =&v[0];
```

报错！！！

1. `"std::_Bit_reference *"`类型的值不能用于初始化 "bool \*" 类型的实体
2. cannot convert 'std::vector<bool>::reference*' to 'bool*' in initialization
3. taking address of rvalue [-fpermissive]

报错原因：
出于空间优化的原因，C++标准（早在 C++98）显式地调用 vector<bool> 作为特殊的标准容器，其中每个 bool 仅使用一位空间，而不是普通 bool 的一个字节（实现一种“动态位集”）。
作为这种优化的交换，它*不提供*普通标准容器的所有功能和接口。

在这种情况下，由于您不能获取字节中的位的地址，因此诸如 operator[] 之类的东西不能返回 bool& ，而是返回一个允许操作所讨论的特定位的代理对象。
因为这个代理对象不是 bool& ，所以你*不能*像在一个“普通”容器上调用操作符那样，将它的地址分配给 bool* 。这意味着 bool *pb =&v[0]; 不是有效代码。

- 顺带一提 `deque`就没这个现象：
  deque 没有任何这样的特殊化，所以每个 bool 都需要一个字节，你可以从 operator[] 返回值的地址。

[上述内容参考链接](https://stackoverflow.com/questions/17794569/why-isnt-vectorbool-a-stl-container)

#### 上述的小结

- std::vector<bool>是一个特殊的容器，它使用比特位打包来存储布尔值。
- 使用 auto 关键字时，需要小心 std::vector<bool>的 operator[]返回的代理对象，因为它可能导致悬空引用问题。
- 在某些情况下，显式声明变量类型比使用 auto 更安全，尤其是在处理可能涉及*悬空引用*或*代理对象*的类型时。

* 会出现这种错误 还是因为程序员对 vector<bool>了解的不够充分 如果

#### 扩展：C++代理类及其在标准库中的应用与设计考量

1. 代理类的概念
   定义：代理类是一种设计模式，它用于模拟或增强某些类型的行为。
   用途：代理类常用于访问控制、延迟计算、或提供对复杂数据结构的简化接口。

2. std::vector<bool>::reference 作为代理类
   模拟了对 std::vector<bool>中单个比特位的引用。
   不可见性：这个代理类对用户是*透明*的，用户不需要直接与代理类交互，而是通过 std::vector<bool>的 operator[]*间接*使用。

3. 智能指针作为代理类的例子
   std::shared_ptr 和 std::unique_ptr：这些智能指针也是代理类，它们控制着原始指针的生命周期，并提供自动内存管理。
   可见性：与 std::vector<bool>::reference 不同，智能指针对用户是*可见*的，并且它们的行为和接口是用户所熟知的。

4. 代理类与 `auto` 关键字
   不推荐：不可见的代理类通常不适用于 auto 关键字，因为它们的*生命期通常不会超过一条语句*。

5. 代理类的生命期
   限制：代理对象通常不应该在表达式之外存活，因为它们可能持有悬空指针或引用。
   未定义行为：如果代理对象的生命期超出了设计预期，可能会产生悬空引用或指针，导致未定义行为

6. 代理类的隐式转换
   std::vector<bool>::reference：存在一个从 std::vector<bool>::reference 到 bool 的隐式转换。
   自定义代理类：对于自定义的代理类，如矩阵运算中的代理类，也可以定义隐式转换，允许代理对象在表达式中使用，并在需要时转换为最终结果类型。

7. （拓展 可不看）[自定义代理类](../../9.expertPart/9.3代理类.md)

- 实际上， 很多开发者都是在跟踪一些令人困惑的复杂问题或在单元测试出错进行调试时才看到代理类的使用。

但是学完上一章 我们了解到很多`auto`好处 那就要因为代理类而放弃它吗？不！遇到代理类我们还能用`static_cast`

```cpp
auto highPriority = static_cast<bool>(features(w)[5]);
auto sum = static_cast<Matrix>(m1 + m2 + m3 + m4);
```

**请记住：**

- 不可见的代理类可能会使`auto`从表达式中推导出“错误的”类型
- 显式类型初始器惯用法强制`auto`推导出你想要的结果
