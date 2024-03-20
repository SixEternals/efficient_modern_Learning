#include <iostream>
using namespace std;

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

void test1()
{
    A(123); // &&
    int x = 123;
    A(x); // &

    A("123"); // &
    string str = "123";
    A(str); // &

    A(x++); // &&
    A(++x); // &
}

class B
{
    int value;

public:
    B(int x) : value(x) { cout << "default constructor" << endl; }
    B(const B &obj) : value(obj.value)
    {
        cout << "copy constructor" << endl;
    }
    B(const B &&obj) { cout << "move constructor" << endl; }
    B operator+(const B &other)
    {
        return B(value + other.value);
    }
    ~B() { cout << "destructor" << endl; }
};
void test2()
{
    // lvalue reference
    int x = 123;
    int &rx = x;
    cout << "address must be same:x:" << &x << " rx:" << &rx << endl;

    // rvalue reference
    B b1(1);
    B b2(2);
    b1 + b2;

    // B b3(b1 + b2); // 创建一个B的临时对象
    // B &&b3 = b1 + b2;
    // B b4 = b1 + b2;

    cout << "================================" << endl;
}

// void func(int x) { cout << "int" << endl; }
void func(int &x) { cout << "int&" << endl; }
void func(int &&x) { cout << "int&&" << endl; }
void test3()
{
    int x;
    int &y = x;

    func(x);
    func(y);

    func(123);
}

void test4()
{
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
}
int main(int argc, char const *argv[])
{
    // test1();

    test2();

    // test3();

    // test4();
    return 0;
}
