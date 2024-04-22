#include <iostream>
#include <memory>
using namespace std;
class Interger
{
public:
    explicit Interger(int val) : value(val) {}
    int get() const { return value; }
    void set(int val) { value = val; }

private:
    int value;
};
class IntegerProxy
{
public:
    explicit IntegerProxy() noexcept {}
    explicit IntegerProxy(std::shared_ptr<Interger> ptr) : _ptr(ptr) {}

    int get() const noexcept
    {
        return _ptr->get();
    }
    void set(int val) noexcept { _ptr->set(val); }
    int operator*() const { return get(); }

private:
    std::shared_ptr<Interger> _ptr;
};

int main(int argc, char const *argv[])
{
    auto interger = std::make_shared<Interger>(10);
    IntegerProxy proxy(interger);

    // 使用代理对象
    cout << "Current value:" << proxy.get() << endl;
    proxy.set(20);
    cout << "New value:" << proxy.get() << endl;
    return 0;
}

/*
要实现类似于表达式模板的行为，您需要创建一个代理类，该类能够“代理”复杂的表达式，并且能够以一种高效的方式进行计算。在您提供的代码中，`IntegerProxy` 类已经是一个简单的代理类，它代理了一个 `Interger` 对象的操作。为了模拟表达式模板的行为，我们需要对 `IntegerProxy` 进行扩展，使其能够处理更复杂的表达式，比如支持多个 `IntegerProxy` 对象的加法操作，并且能够以惰性的方式（lazy evaluation）存储这些操作直到实际需要结果时才执行计算。

以下是实现表达式模板的一个基础示例，以您的 `IntegerProxy` 类为基础进行扩展：

```cpp
#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Integer {
public:
    explicit Integer(int val) : value(val) {}
    int get() const { return value; }
    void set(int val) { value = val; }

private:
    int value;
};

class IntegerProxy {
public:
    explicit IntegerProxy() noexcept {}
    explicit IntegerProxy(shared_ptr<Integer> ptr) : _ptr(ptr) {}

    int get() const noexcept {
        return _ptr->get();
    }
    void set(int val) noexcept {
        _ptr->set(val);
    }

    IntegerProxy& operator+=(const IntegerProxy& other) noexcept {
        if (_representation.empty()) {
            _representation = "( " + to_string(_ptr->get());
        } else {
            _representation += " + " + to_string(_ptr->get());
        }
        return *this;
    }

    // 重载解引用运算符，以支持 *proxy 语法
    int operator*() const {
        return evaluate();
    }

    // 惰性计算并获取结果
    int evaluate() const {
        if (!_representation.empty()) {
            // 这里应该添加实际的计算逻辑，这里只是简单地返回第一个操作数作为示例
            return _ptr->get();
        }
        return 0;
    }

    // 显式构造函数，防止隐式类型转换
    explicit operator int() const {
        return evaluate();
    }

private:
    shared_ptr<Integer> _ptr;
    string _representation; // 用于存储表达式字符串，实际使用中应实现完整的表达式解析和计算
};

int main() {
    auto interger1 = make_shared<Integer>(10);
    auto interger2 = make_shared<Integer>(20);
    IntegerProxy proxy1(interger1);
    IntegerProxy proxy2(interger2);

    // 使用代理对象进行加法操作
    proxy1 += proxy2;

    // 通过显式类型转换获取最终结果
    int sum = static_cast<int>(proxy1);
    cout << "Sum: " << sum << endl;

    return 0;
}
```

在这个示例中，`IntegerProxy` 类被扩展为支持 += 操作符，它将加法操作以字符串形式存储起来，而不是立即执行计算。这样做的目的是延迟计算，直到我们真正需要结果的时候。在实际应用中，您需要实现一个完整的表达式解析器和求值器来计算这些延迟的表达式。

此外，我们重载了 `operator*` 和 `explicit operator int()` 来支持通过解引用和显式类型转换来获取 `IntegerProxy` 对象的值。

请注意，这个示例仅用于演示表达式模板的基本概念，实际的表达式模板实现会更复杂，需要考虑更多的操作符和优化。

*/