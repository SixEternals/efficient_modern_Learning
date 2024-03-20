# 基础 5 lambda 表达式基础部分

[对应源文件](./src/lambda.cpp)
写这个 md 只是作总结 详细看 cpp 文件

## lambda 结构

用[]来指定捕获内容 [&]捕获上下文所有变量而且传入引用 [=]和前面同理但是传值 [&,=a]其他传引用但是 a 传值
还能用[this]和[&]捕获捕获 class 的 this 指针

用()传入参数

noexcepct 为可选选项

lambda 可以指定返回值 除了返回`initializer_list`之外 都可以省略

修饰符 可以用 mutable 来指定 lambda 里可以修改捕获变量

## lambda 本质是类

## lambda 最有用的一集

在实际项目中见过的一段

```cpp
int x;
int copy_x = x * 2;
.... 省略一大段代码 在这之中x已经改变
copy_x = x * 2; // 这里还需要拿新的x值来重新赋值 很多时候都会忘记写这句
```

看起来 copy_x 再赋值一次好像不容易被忽略啊 实际上写太长了可能会忘记重新赋值 于是用上了 x 的旧值 那 copy 副本有啥用？？
这时用 lambda 就可以轻松应对

```cpp
int x = 100;
auto copy_x = [&x]{return x * 2};
```

这样写就能一直追踪 x 的最新值了

## 闭包定义及其用法

闭包意为函数内包含了上下文消息

## function 部分和 bind 函数仅作了解
