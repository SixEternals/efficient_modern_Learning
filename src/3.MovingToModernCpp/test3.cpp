#include <iostream>
#include <vector>
#include <type_traits>
using namespace std;
template <typename T>
decltype(auto) add(T a, T b)
{
    return a + b;
}

template <typename T>
class Stu
{
public:
    Stu() = default;
    using MyVector = vector<T>;
    void show(MyVector &v) const
    {
        for (auto i : v)
        {
            cout << i << " ";
        }
        cout << endl;
    }
};

struct s1
{
    int Aname;
};
struct s2
{
    typedef int Aname;
};
struct s3
{
    static int Aname;
};
template <typename T>
class Myclas
{
public:
    void func()
    {
        typename T::Aname *ptr;
    }
};

template <typename T>
struct Myvector
{
    typedef vector<T> type;
};
void test2()
{
    // type<int> vec; // 报错!
    Myvector<int>::type vec; // 这一坨又长又臭 而且使用起来又诸多不便
}
template <typename T>
class c2
{
public:
    c2() = default;
    void func()
    {
        typename Myvector<T>::type Cvec;
    }
};
// __________________________
template <typename T>
struct myStruct
{
    typedef vector<T> Myvector;
};
template <typename T>
class testClass
{
public:
    testClass() = default;
    void func()
    {
        typename myStruct<T>::Myvector vec;
    }
};

// ——————————————————————————————
template <typename T>
using myVector_t = vector<T>;

template <typename T>
class testClass2
{
public:
    testClass2() = default;
    void func()
    {
        myVector_t<T> vec;
    }
};

// 用using来给结构体的指针作别名alias
struct Foo
{
    void func(int a, double b) { cout << a << " " << b << endl; }
};


int main(int argc, char const *argv[])
{
    // cout << add(1, 2) << endl;
    // cout << add(1.2, 3.3) << endl;
    // cout << add(1L, 10L) << endl;

    vector<int> vec1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<string> vec2{"nihao", "woc"};
    Stu<int> instance1;
    // instance1.show(vec1);
    Stu<string> instance2;
    // instance2.show(vec2);

    Myclas<s2> test1;
    Myclas<s1> test2;

    c2<int> test3;

    // MyTest3
    testClass<int> instance3;

    // using for alias
    using FooPtr = void (Foo::*)(int, double);
    Foo instance4;
    FooPtr ptr = &Foo::func;
    // (instance4.*ptr)(1, 2.2);

    // type_traits
    const int obj = 1;
        
    return 0;
}
