#include <iostream>
using namespace std;

class A
{
public:
    string name;
    A(string name) : name(name) {}
    void access()
    {
        cout << name << endl;
    }
};

int main(int argc, char const *argv[])
{
    A a{"test"};
    a.access();

    auto b = move(a);
    b.access();

    cout << a.name << endl;
    return 0;
}
