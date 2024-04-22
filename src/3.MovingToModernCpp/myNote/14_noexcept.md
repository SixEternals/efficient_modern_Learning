# 如果函数不抛出异常 请使用`noexcept`

版本：C11
noexcept 运算符是一个编译时运算符

## 使用`noexcept`理由 1--确保函数不会抛出异常后 添加`noexcept`来优化

前提：保证函数体内绝对没有异常被抛出
展示：

```cpp
RetType function(params) noexcept;  //极尽所能优化 此为C11语法
RetType function(params) throw();   //较少优化 此为C98语法
RetType function(params);           //较少优化
```

它可以进行一些优化，避免生成与异常处理相关的额外代码。
如果不加`noexcept`的话 那编译器就会自动产生许多检查代码 用于检查是否抛出异常等

## 用于移动语义

比如`vector` 来说

1. 在 C98 中 新元素添加的原理就是：当前`vector`空间不足时 就会开辟一片新的更大的空间 然后把旧元素给**复制**到新区域中 然后老内存区*每个*元素发生狗系 这使得`push_back`可以提供很强的异常安全保证。
   如果在复制元素期间抛出异常 则旧区域元素保持不变即可。
2. 在 C11 中 一个很自然的优化就是将上述复制操作替换为**移动**操作 但是就是这一换 唤出了大问题。这回破坏`push_back`的异常安全保证
   假如已经有 n 个元素已经从老内存区移动到新内存区了 但是异常在第 n+1 个元素时抛出.此时因为已经有 n 个元素被移走了 所以也不可能恢复原样了(重新移回去的时候可能又出异常了...)

- 此时的解决方法为：同时有复制构造和移动构造 然后使用各种检查(这个补充在下面代码里) 检查出确实不跑出异常 那就使用移动 否则使用复制

`=============================================================`
在 C++中，noexcept 关键字用于声明函数不会抛出任何异常。当你在一个函数（比如移动构造函数）后面使用 noexcept，你是在告诉编译器这个函数保证不会抛出异常。如果它违反了这个保证，程序将调用 std::terminate，这通常会导致程序立即终止。

对于 std::vector::push_back 而言，noexcept 的使用是有条件的。std::vector::push_back 接受一个右值引用作为参数，这个参数可以是一个对象或者一个对象的移动引用。在 C++11 及以后的版本中，如果元素类型支持移动语义，std::vector::push_back 会尝试使用移动操作来提高效率。

这里的关键点是 std::move_if_noexcept，它是 C++11 引入的一个条件移动操作。std::vector::push_back 内部可能会使用 std::move_if_noexcept 来决定是否使用移动操作。std::move_if_noexcept 的工作原理如下：

如果传递给 std::move_if_noexcept 的对象类型具有 noexcept 的移动构造函数，那么 std::move_if_noexcept 将返回一个右值引用，允许对象被移动。
如果对象类型没有 noexcept 的移动构造函数，或者没有移动构造函数，std::move_if_noexcept 将返回一个左值引用，表示对象不能被安全地移动。
因此，当你声明一个类型的移动构造函数为 noexcept 时，你实际上是在告诉编译器这个类型的移动构造函数是安全的，不会抛出异常。这样，当 std::vector::push_back 需要复制或移动一个元素时，如果元素的移动构造函数是 noexcept 的，它将使用移动操作而不是复制操作，从而提高性能。

这里是一个简化的例子，说明如何为一个类型声明 noexcept 的移动构造函数：

```cpp
class MyType {
public:
    // 移动构造函数
    MyType(MyType&& other) noexcept : data(nullptr) {
        // 使用 std::swap 来交换数据指针，这是一种常见的实现移动语义的方式
        // std::swap 在两个整数之间工作得很好，并且不会抛出异常
        std::swap(data, other.data);

        // 如果 data 是一个复杂的资源，需要手动转移所有权
        // data = other.data; // 转移资源的所有权
        // other.data = ...;   // 将 other 的相应成员变量设置到一个有效但未使用的状态
    }
    /* std::swap 对于内置类型（如整数、指针等）是 noexcept 的，这意味着它不会抛出异常。如果 data 是一个复杂的资源，你需要手动实现资源的转移，并确保 other 对象在移动构造函数结束后处于有效但未使用的状态。*/
    // 其他成员和方法
private:
    int* data; // 假设 data 是一个指向动态分配整数的指针 并且已经是一个支持移动语义的类型
    // 故在对data操作时不需要添加`std::move_if_noexcept`
};
// 注意：在实际使用中，MyType 的其他部分也需要支持移动语义，包括移动赋值操作符和析构函数，以确保整个类的移动语义是安全的。
```

在这个例子中，MyType 的移动构造函数使用 noexcept 声明，这表明它保证不会抛出异常。如果 std::vector<MyType>的 push_back 操作需要复制或移动 MyType 对象，它将*安全地使用移动操作*，因为移动操作是 noexcept 的。

身边的例子就是`vector`，标准库中例如`std::vector::reverse`,`std::deque::insert`等等 都收哦这种强异常保证

### std::move_if_noexcept 讲解

这个函数主要用于库作者在编写通用代码时，以一种安全的方式处理可能的移动操作。
上一份代码中 由于只需要对 data(假设其是一个支持移动语义的类型)进行操作 而且在该构造函数中使用到的`swap`本来就保证了异常安全 所以更加不需要`move_if_noexcept`了

然而，在一些情况下，可能需要使用 std::move_if_noexcept 来安全地执行条件性移动。例如，编写一个模板函数或类，并且**不确定模板参数类型 T 的移动构造函数是否抛出异常**，那么使用 std::move_if_noexcept 可以确保您的代码不会在尝试移动时抛出异常：

```cpp
template<typename T>
void safeMove(T& from, T& to) {
    to = std::move_if_noexcept(from);
}
```

在这个例子中，如果 T 的移动构造函数是 noexcept 的，std::move*if_noexcept 将执行移动操作；*否则\_，它将执行复制操作。这确保了 safeMove 函数在面对可能抛出异常的移动构造函数时仍然安全。

总结来说，是否使用 std::move_if_noexcept 取决于您的具体需求和上下文。在一些通用编程场景中，它提供了一种安全处理移动操作的方式。在其他情况下，如 MyType 的移动构造函数示例中，如果已知特定类型支持 noexcept 移动，您可能会直接使用 std::swap 或直接赋值来实现移动语义。

## stl::swap 函数和 std::容器的 swap

上面引入已经说到了 `swap`本身就保证了抛出安全

容器的自定义 swap
对于 STL 容器，如 std::vector，它们通常会提供自己的 swap 成员函数，这些函数通常比通用 swap 更高效，因为它们可以利用容器内部的数据结构和算法来执行交换操作。

例如，std::vector 的 swap 成员函数会交换两个向量的内部数据指针、分配器和容量等成员，而不需要逐个交换向量的每个元素。

```cpp
template <class T, class Allocator>
void vector<T, Allocator>::swap(vector<T, Allocator>& other) {
using std::swap;
swap(this->data*, other.data*);
swap(this->size*, other.size*);
swap(this->capacity*, other.capacity*);
// ... 可能还有其他成员的交换 ...
}
```

在这个例子中，using std::swap;语句使得成员函数可以使用 STL 的通用 swap 函数来交换成员变量。这样做的好处是，如果成员变量的类型也有特别的 swap 函数，STL 的通用 swap 会调用那个特别的版本，从而提高交换的效率。

## 到底什么时候能用？

1. 给叶子函数添加绝对是万无一失(简单的 leaf function，像是 int，pointer 这类的 getter，setter 用 noexcept。因为不可能出错)

2. 用在析构函数 如果连析构函数都抛出异常了 那程序本来就会因为内存释放失败而终止 而不是因为你标记了`noexcept`而终止 所以说用了也没问题

3. 若一个函数调用了一个可能会抛出异常的函数 那就不要用`noexcept`

4. move constructor/assignment operator 如果不会抛出异常，一定用 noexcept。(这个例子为 vector 的移动构造函数 什么已经有了)

5. 默认不使用 很多时候都预测不了看不见的 allocation。
   如果乱加 可能会导致以后程序莫名其妙终止了又看不到错误日志 那就连 bug 都找不到在哪了

6. `noexcept`属于优化项 在开发阶段如果做到上面的 1 2 4 点后就基本可以不用写了。等到优化阶段再考虑。

7. 比较适合底层库作者使用：
   ① 用的地方多，性能能抠一点是一点，一点改进可能被放大好多倍
   ② 同样因为用的地方多，如果错了，问题容易暴露
   ③ 处于调用栈的底层，环境比较可控，功能更单一，容易实施
   ④ 面对的变更比较少，为此在上面花时间去做优化可能值得

### 好处

1. 可读性强 和`constexpr`一样 程序员一眼就知道这个函数想要干嘛
2. 不仅有助于提高代码的异常安全性和效率，而且允许编译器生成更优化的代码。
3. 减小编译后的文件体积

### 滥用弊端

1. 当函数被标记`noexcept`时 如果真的出现了异常 那么程序直接终止 异常日志不会被打印 相当于你也不知道哪里错了
2. 通常情况下，在广泛使用 STL 容器，智能指针的现代 C++风格下，编译器能够推导自动生成的析构函数，move 构造和赋值运算符的 noexcept 属性
3. noexcept 判断比较复杂，业务代码程序员更关注业务逻辑本身，而且需求变化大，代码可能很复杂，人工判断很容易出错。
4. noexcept 会影响接口的灵活性，比如基类某个虚函数设置为 noexcept，派生类覆盖虚函数时也必须遵守，这个有些情况下难以保证
5. noexcept 用错了危害很大，会强行调 std::terminate，本来能处理的都没有处理机会了就像异常规格的存在版本问题一样，如果一个函数从 noexcept 变为 noexcept(false)，调用处可能也需要跟着改动
6. C++17 后，noexcept 还影响了函数的签名，进一步影响了代码的复杂性和兼容性

[参考链接](https://www.zhihu.com/question/30950837)
