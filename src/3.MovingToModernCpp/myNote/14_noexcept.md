# 如果函数不抛出异常 请使用`noexcept`

版本：C11
noexcept 运算符是一个编译时运算符

## 什么时候使用 noexcept 操作符？

1. You have the guarantee that the function will not throw any exception in any case. 你可以保证函数在任何情况下都不会抛出任何异常。

2. You want to optimize the performance by allowing the compiler to skip the process of exception handling which leads to faster execution of the program. 您希望通过允许编译器跳过异常处理过程来优化性能，从而加快程序的执行速度。

3. **正确性优先**：在确保函数真正不会抛出任何异常的情况下才使用 noexcept。如果仅为了优化而错误地使用 noexcept，可能会隐藏异常，导致程序复杂化和难以维护。

### 好处

1. 可读性强 和`constexpr`一样 程序员一眼就知道这个函数想要干嘛
2. 不仅有助于提高代码的异常安全性和效率，而且允许编译器生成更优化的代码。

## 什么时候不使用 noexcept 操作符？

1. Your function can throw exceptions, or if you’re not sure about its exception safety. 你的函数可以抛出异常，或者如果你不确定它的异常安全性。
2. When you are dealing with destructors as noexcept destructor can lead to undefined behavior, when exceptions are thrown during object destruction. 当你处理析构函数时，当对象析构过程中抛出异常时，析构函数会导致未定义的行为。

### 滥用弊端

当函数被标记`noexcept`时 如果真的出现了异常 那么程序直接终止 异常日志不会被打印 相当于你也不知道哪里错了

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

## swap 函数

上面引入已经说到了 `swap`本身就保证了抛出安全
