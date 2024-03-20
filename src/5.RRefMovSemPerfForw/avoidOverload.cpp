#include <iostream>
#include <vector>
using namespace std;

class Test
{
public:
    Test() = default;
    Test(const Test &other) {}
    Test(Test &&other) {}
};

template <typename T>
void f1(T &&a) { cout << "void f1(T &&a)" << endl; }

void f1(const Test &a) { cout << "void f1(const Test &a)" << endl; }
void f1(Test &&a) { cout << "void f1(Test &&a)" << endl; }

vector<string> vec;
template <typename T>
void logAndAdd(T &&name)
{
    cout << "in T && function" << std::endl;
    vec.emplace_back(name);
}

void logAndAdd(int id)
{
    cout << "In Id func" << endl;
}

class Person
{
    string name;

public:
    Person() = default;
    // Person(string name) : name(name) { cout << "Person(string name)" << endl; }
    Person(const string &name) : name(name) { cout << "Person(const string &name)" << endl; }

    template <typename T>
    Person(T &&name)
    {
        cout << "Person(T &&name)" << endl;
    }

    Person(string &&name)
    {
        this->name = move(name);
        cout << "Person(string &&name)" << endl;
    }
};
class SpecialPerson : public Person
{
public:
    SpecialPerson() = default;
    SpecialPerson(const SpecialPerson &lhs) : Person(lhs) {}
    SpecialPerson(SpecialPerson &&rhs) : Person(rhs) {}
};
void func()
{
    Person p1{"nohao"};
    Person p2("soidh");
    string s = "sd";
    Person p3(s);
    cout << "=============" << endl;
    SpecialPerson sp1{};
    SpecialPerson sp2 = sp1;
    SpecialPerson sp3 = move(sp1);
}

int main(int argc, char const *argv[])
{
    Test t{};
    f1(t);
    cout << "===============" << endl;

    std::string petName("Darla");         // 跟之前一样
    logAndAdd(petName);                   // 跟之前一样，拷贝左值到multiset
    logAndAdd(std::string("Persephone")); // 移动右值而不是拷贝它
    logAndAdd("Patty Dog");               // 在multiset直接创建std::string 而不是拷贝一个临时std::string
    logAndAdd(12);
    cout << "======" << endl;
    // logAndAdd(static_cast<short>(12));

    func();
    return 0;
}
