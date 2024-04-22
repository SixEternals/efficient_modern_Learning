## 条款四十一：对于移动成本低且总是被拷贝的可拷贝形参，考虑按值传递

### 引入

有些函数的参数从设计上就是要被 “拷贝” 的，例如将参数拷贝加入类的容器中，具体可能是通过拷贝或移动的操作实现。为了效率，一般应该对左值参数做拷贝，右值参数做移动，例如下面的代码：

```cpp
class MyString {
public:
    MyString() : str("default MyString.") {}
    MyString(const MyString& rhs) : str(rhs.get());
    MyString(MyString&& rhs) : str(std::move(rhs.get()));
    std::string get() const { return str; }
private:
    std::string str;
};

class Widget {
public:
	Widget() { names.reserve(5); } // 为保证后面实验中 push_back 不出现 vector 空间不足扩容导致出现额外的拷贝，先预留一些空间

    void addName(const MyString& newName) {
        names.push_back(newName);
    }
    void addName(MyString&& newName) {
        names.push_back(std::move(newName));
    }
private:
    std::vector<MyString> names;
};
// 调用
Widget w;
MyString s;
w.addName(s); // 拷贝构造
w.addName(std::move(s)); // 移动构造
```

这从功能上没有问题，效率也很好，但是有一点麻烦：需要声明、实现、文档记录、维护两个功能几乎相同的函数，而且编译的目标码中也会存在两份函数。一种替代方式是使用万能引用参数：

```cpp
class Widget {
public:
	template<typename T> // take lvalues and rvalues;
	void addName(T&& newName)
	{
		names.push_back(std::forward<T>(newName)); // copy lvalues, move rvalues;
	} // see Item 25 for use of std::forward
};
```

- 但是这样的缺点也很明显：允许的*参数范围太大*了

- 如果入参是不合适的类型，那么编译器产生的*错误信息可能非常晦涩难懂*

- 另外*目标码中仍会存在两份甚至更多份函数*。

那么有没有一种一个函数解决两种情况的方案呢？答案就在本节的标题中：考虑 按值传递（pass-by-value）。

```cpp
class Widget {
public:
    void addName(MyString newName) {
        names.push_back(std::move(newName));
    }
private:
    std::vector<MyString> names;
};
```

### 重载 vs 通用引用 vs 传值引用

```cpp
class Widget {                                  //方法1：对左值和右值重载
public:
    void addName(const std::string& newName)
    { names.push_back(newName); } // rvalues
    void addName(std::string&& newName)
    { names.push_back(std::move(newName)); }
    …
private:
    std::vector<std::string> names;
};

class Widget {                                  //方法2：使用通用引用
public:
    template<typename T>
    void addName(T&& newName)
    { names.push_back(std::forward<T>(newName)); }
    …
};

class Widget {                                  //方法3：传值
public:
    void addName(std::string newName)
    { names.push_back(std::move(newName)); }
    …
};
```

考虑这两种调用方式：

```cpp
Widget w;
…
std::string name("Bart");
w.addName(name);              //传左值
…
w.addName(name + "Jenne");    //传右值
```

提示：`operator+`的返回值是右值
解释：

1. 重载版本：左值一次拷贝 右值一次移动
2. 通用引用版本：左值一次拷贝 右值一次移动
3. 按值传递：左值一次拷贝一次移动 右值两次移动；均多出一次移动操作

### "对于移动成本低且总是被拷贝的可拷贝形参，考虑按值传递"什么意思？

本节标题的描述方式其实蕴含了以下 4 条信息：

1. 你可以 **考虑** 使用按值传参，但这不是一个强制性的意见，因为这样做毕竟在开销上是比前两种方法大的。

2. 只对 **能够被拷贝的参数** 考虑按值传参。对于 move-only 的类型，实际上根本不需要写两个重载函数，只用右值参数版本即可，也没有任何理由用开销更大的按值传参了。

3. 只对 **移动开销很小的类型** 考虑按值传参。以上分析已经展示了按值传参与其它两种方法的开销差异：多一次移动操作。如果参数类型的移动操作开销很大，那么用这种方式就是得不偿失了。

4. 只对 **一定会被拷贝** 的参数考虑按值传参。如果函数从逻辑上不一定会拷贝参数（例如上面的例子，如果逻辑是先检查字符串的长度，如果在一定区间内才进行插入），甚至大部分时候都不会，那么以上的开销对比就会变成 一次拷贝或移动 + 一定几率一次移动 VS 一定几率一次拷贝或移动，差距更大了。

#### 即使 2、3、4 的条件都满足，考虑是否使用按值传参时也还需要注意三个问题：

1. 如果调用不只一层，而是一条较长的 调用链，那么每层一次额外的移动操作可能就是无法接受的，对于性能要求严格的应用尤其如此。

2. 拷贝赋值与拷贝构造有区别。以上分析是针对拷贝构造，拷贝赋值的情况更加复杂，在有些情况下按值传参与重载之间的开销差距会 更大，具体与左值版本函数被调用的比例、类本身是否使用动态分配（堆内存）、其它优化设计等等有关。

3. 如果入参可能是参数类型的 派生类对象，那么就不能使用按值传参，因为会引发对象的 切割（slicing），入参对象只能保持基类的部分。这本质上就是 C++ 值语义没有多态性 的体现。虽然这条与效率无关，但至关重要，直接影响程序的正确性。

## 补充：拷贝构造函数 ≠ 拷贝复制

```cpp
MyClass obj1;
MyClass obj2 = obj1;  // 使用拷贝构造函数
```

```cpp
MyClass obj1;
MyClass obj2;
obj2 = obj1; // 使用拷贝赋值操作符
```

### 总结

1. 对于可以而且一定会被拷贝、移动开销很小的参数，按值传参的性能几乎与按引用传参相同，实现上更简单，而且生成的目标码体积更小。
2. 拷贝赋值可能比拷贝构造的开销大很多（原书这里疑似写反了）。赋值 bad ，构造 good 除非你自己写的重载`operator+` (自定义的拷贝构造函数可以实现深拷贝，而自定义的赋值操作符可以实现更高效的赋值逻辑，如通过交换技术（swap）来避免不必要的复制。)
3. 由于对象切割问题的存在，按值传参一般不适合用于基类类型的参数。
   ∵ 即派生类对象的额外成员*不会*被拷贝到基类对象中。这是因为*按值传递会创建一个基类类型的副本*，而这个副本不包含派生类的信息。

### 参考链接

[参考链接](https://blog.csdn.net/Altair_alpha/article/details/123943272)
