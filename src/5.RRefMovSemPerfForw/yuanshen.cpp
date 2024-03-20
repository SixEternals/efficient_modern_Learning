#include <iostream>
using namespace std;
template <typename T>
struct Func1
{
    using type = T;
};

// template <int a>
// struct Func2
// {
//     int data[a];
// };

class Person
{
public:
    Person() = default;
    template <typename T, typename = typename Func1<T>::type>
    Person(T &&n)
    {
        cout << "Person(T &&n)" << endl;
    }
    Person(const Person &p) { cout << "Person(const& p)" << endl; }
};
int main(int argc, char const *argv[])
{
    Func1<int>::type value = 10;

    // Func2<1> f;
    // &f;
    // cout << &f;

    Person p;
    Person p1(p);
    return 0;
}
