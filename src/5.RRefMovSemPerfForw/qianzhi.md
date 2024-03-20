# 合订本

## 左右值是什么

左值全称 `locator value`(可寻址的数据) 右值全称 `read value`(不可寻址/只读数据)

```cpp 左值举例
++x; // 返回x自增后的引用
y *= 3;
```

```cpp 右值举例
x++; // 返回x的旧值得临时副本 关于这个问题记得在stack overflows论坛里面见过回答，，，找不到连接了 找到再说
y * 3;
```

以上说法参考至[https://stackoverflow.com/questions/50802859/why-is-x-a-lvalue-and-x-a-rvalue]
(打不开连接的省流版 ↓)
按照伪码写法

```cpp
int& preFix(int& OriginVal){
    int &newVal = OriginVal;
    newVal += 1;
    return newVal;
}

int PostFix(int& OriginVal){
    int oldvalue = OriginVal;
    OriginVal += 1;
    return oldvalue;
}
```

++x 的实现方式类似于上述伪码 直接返回它自己就行了 即左值引用 int&;但是 x++又要实现返回旧值又要自增 那么只能返回右值 oldvalue 了

### 简单判别是否左值

```cpp 加个取址符号即可
&x; // √
&123; // x 芝士右值
&"ceshi"; //√
```

### 补充 所有字面量都是右值 除了字符串

字面量举例：123, 12.5, true...
唯独字符串是左值
原因：字符串本质是数组(在 c 是 char [],c++是 const char[]), 数组本身就不能作为右值
[https://stackoverflow.com/questions/10004511/why-are-string-literals-l-value-while-all-other-literals-are-r-value]

代码验证

```cpp
#define A(x) _A(x, "func(" #x ")")

template <typename T>
void _A(T &x, string str)
{
    cout << str << "is T &x" << endl;
}

template <typename T>
void _A(T &&x, string str)
{
    cout << str << "is T &&x" << endl;
}
int main(int argc, char const *argv[])
{
    A(123); // &&
    int x = 123;
    A(x); // &

    A("123"); // &
    string str = "123";
    A(str); // &

    A(x++); // &&
    A(++x); // &
    return 0;
}
 // result
func(123)is T &&x
func(x)is T &x
func("123")is T &x
func(str)is T &x
func(x++)is T &&x
func(++x)is T &x
```

## 左右值引用是什么？

左值引用得到的还是左值

```cpp
void func(int &x) { cout << "int&" << endl; }
void func(int &&x) { cout << "int&&" << endl; }
void test3()
{
    int x;
    int &y = x;
    func(x); // int&
    func(y); // int&
    func(123); // int&&
}
```

引用的意思就是给变量起个别名 两者地址都会一样

```cpp
int x;
int& y = x;
&x === &y; // the same thing
```

右值引用的定义:(wait for...)
右值引用可以延长分配给他们的临时对象的生命周期
对于非常量引用右值引用还能修改值

特别说明 右值可以绑定给 const 的左值引用 而左值不可以绑定给右值引用

```cpp
    // int &x = 123; // X 非const左值引用不能绑定 右值
    const int &x = 123; // √

    int value = 123;
    int &&y = 123;
    int &&z = move(value); //
    int &&q = static_cast<int>(value);
    double d = 12.2;
    int &&m = static_cast<int>(d); // 显示转换肯定行
    int &&n = d;                   // 这里可以绑定是因为 d是double 而n要接受一个int 就会让d隐式转换为int 而转换结果恰恰为一个临时值(右值) 故可绑定

    // int &&k = value; // 出错

```

参考至
https://www.geeksforgeeks.org/lvalues-references-and-rvalues-references-in-c-with-examples/

右值引用的使用场景 include 移动构造函数 完美转发 自定义类型的算数运算

## 移动构造函数

当一个对象从一个作用域移动到另外一个作用域时 使用移动 constructor 可以避免 copy

for example

```cpp
class B{
    int value;
public:
    B(int x) : value(x){cout << "default" << endl;}
    B(const B& obj) :value(obj.value){cout << "copy"  <<endl;}
    B(B&& ob):valuej(obj.value){cout << "move" << endl;}
    B operator+(const B& other){return value+obj.value;}
};
B b1(10);
B b2(20);

b1 + b2;

```

这段会打印 move(省略其他的打印内容) Ⅰ

而如果注释掉移动构造函数的话 仅保留拷贝构造器的话 仅会打印 copy Ⅱ

先解释 Ⅰ:

## 完美转发

## 自定义类型的算数运算

## move
