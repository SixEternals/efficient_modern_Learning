#include <iostream>
#include <variant>
#include <string>
#include <chrono>
using namespace std;
// int g(int i)
// {
//     return 10;
// }
// std::string g(int i)
// {
//     return "hello";
// }
template <int i>
auto f()
{
    if constexpr (i == 0)
    {
        return 10;
    }
    else if constexpr (i == 1)
    {
        return "nihao";
    }
}
static constexpr int b = 10;

constexpr int fib(int n)
{
    if (n == 1 || n == 2)
        return 1;
    unsigned int a{0}, b{1}, c{1};
    for (unsigned int i = 3; i <= n; ++i)
    {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}
// template <int N>
// constexpr int fib = (N == 1 || N == 2) ? 1 : fib<N - 1> + fib<N - 2>;
int FIB(int n)
{
    if (n == 1 || n == 2)
        return 1;
    unsigned int a{0}, b{1}, c{1};
    for (unsigned int i = 3; i <= n; ++i)
    {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

class ConstRxprTest
{
public:
    constexpr ConstRxprTest(int v) : val(v) {}

private:
    int val;
};

class normalTest
{
public:
    normalTest(int v) : val(v) {}

private:
    int val;
};

template <int i>
decltype(auto) mySwitch()
{
    if constexpr (i == 0)
        return 123;
    else if constexpr (i == 1)
        return "nihao";
    else if constexpr (i == 2)
        return (345);
}

int main(int argc, char const *argv[])
{
    // cout << f<0>() << endl;
    // cout << f<1>() << endl;

    // ==
    constexpr int a = 10;
    // constexpr const int *ap = &a;
    constexpr const int *bp = &b;

    static constexpr int aa = 10;
    constexpr const int *paa = &aa;

    static constexpr auto res1 = fib(10); // constexpr版本
    static constexpr const auto tmp = res1;
    auto res2 = FIB(1000);
    // static constexpr const auto tmp2 = res2; // 报错

    // 实验3
    static constexpr ConstRxprTest obj1(5); // 鼠标悬浮obj1上 发现它的备注和 normalTest的不一样
    constexpr const auto *pt = &obj1;
    normalTest obj2(5);

    // 实验4
    cout << mySwitch<0>() << endl;
    cout << mySwitch<1>() << endl;
    cout << mySwitch<2>() << endl;
    // constexpr static decltype(auto) tmpOFswitch = mySwitch<2>();

    // 实验5
    int ex5{100};
    decltype(auto) ex55 = ex5;
    decltype(ex5) &ex555 = ex5;
    decltype(&ex5) ex5555 = &ex5;
    decltype(auto) ex55555 = (ex5);
    return 0;
}
