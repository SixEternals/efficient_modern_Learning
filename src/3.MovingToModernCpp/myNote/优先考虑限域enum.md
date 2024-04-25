# 优先考虑限域`enum`

## 前置知识 tuple

这东西的使用方式类似于记录状态

```cpp
std::tuple<int, string, double> employee; // 记录工号 姓名 工资
employee = make_tuple(1, "wang", 70.99);

cout << get<0>(worker);
```

用处就是 比如创建一个网站 需要记录用户的各种信息 就可以用这个记录 uid userName 所在地 自我介绍...

但是 get 函数不支持迭代 如下

```cpp
for(int i = 0 ; i < 3; ++i)  // 错误的写法
    cout << get<i>(worker); // 是完全不支持任何种类的迭代包括最简单的for循环
```

头文件为`#include <tuple>` 这个头文件还包括如查看内部各项类型的函数 查看 tuple 长度的函数 等等 不介绍了 不是重点

## 限域和非限域`enum`分别是啥

```cpp
enum color1{ //非限域
    red, yellow
};
enum class color2{ // 限域
    blue, green
};
```

## 为什么要优先用限域`enum`

1 限域 enum 可以避免污染命名空间

```cpp
// case 1
enum color1{
    red, yellow
};
auto yellow = 100; // error!在编译器眼里 yellow 已经是声明过的了


//case 2
enum class color2{
    blue, green
};
auto green = 100; // 可以通过
```

2 
