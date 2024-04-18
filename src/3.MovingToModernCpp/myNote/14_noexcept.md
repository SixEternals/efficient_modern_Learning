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
