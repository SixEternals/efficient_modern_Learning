#include <iostream>
#include <string.h>
using namespace std;

class Sample
{
    int id;

public:
    void init(int x) { id = x; }
    Sample() {} // default constructor with empty body

    Sample(Sample &t) // copy constructor
    {
        id = t.id;
    }
    void display() { cout << endl
                          << "ID=" << id; }
};
void test()
{
    Sample obj1;
    obj1.init(10);
    obj1.display();

    Sample obj2(
        obj1); // or obj2=obj1;    copy constructor called
    obj2.display();
}

class student
{
    int rno;
    char name[50];
    double fee;

public:
    student(int, char[], double);
    student(student &t) // copy constructor
    {
        rno = t.rno;
        strcpy(name, t.name);
        fee = t.fee;
        cout << "copy constructor" << endl;
    }
    void display();
};

student::student(int no, char n[], double f)
{
    rno = no;
    strcpy(name, n);
    fee = f;
    cout << "Constructor" << endl;
}

void student::display()
{
    cout << endl
         << rno << "\t" << name << "\t" << fee;
}

struct A
{
    A(int a) { cout << "A(int a)" << endl; }
    A(A &a) { cout << "A(const A& a)" << endl; }
};

int main()
{
    student s(1001, "Manjeet", 10000);
    // s.display();

    student manjeet(s); // copy constructor called
    // manjeet.display();

    cout << endl;
    A a = 10;
    A b(10);
    A c(a);
    A d = a;

    cout << endl;
    test();
    return 0;
}
