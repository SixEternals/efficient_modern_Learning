#include <iostream>
#include <vector>
using namespace std;

template <typename T>
class TD;

class MyClass
{
public:
    MyClass() {}
};

int main(int argc, char const *argv[])
{
    // auto x{0};
    // TD<decltype(x)> xType;

    // decltype(auto) x2{(x)};
    // TD<decltype(x2)> x2Type;

    // vector<bool> x3;
    // TD<decltype(x3)> x3Type;

    // MyClass x4;
    // TD<decltype(x4)> x4Type;

    auto x5{10};
    auto x6{10.1};
    auto x7{MyClass()};
    auto x8{new MyClass()};
    const int x9{1};
    const MyClass x10;
    cout << typeid(x5).name() << endl;
    cout << typeid(x6).name() << endl;
    cout << typeid(x7).name() << endl;
    cout << typeid(x8).name() << endl;
    cout << typeid(x9).name() << endl;
    cout << typeid(x10).name() << endl;

    return 0;
}
