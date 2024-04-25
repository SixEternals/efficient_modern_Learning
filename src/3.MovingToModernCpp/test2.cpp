#include <iostream>
using namespace std;

class MyClass
{
public:
    MyClass(int x) : value(x) {}
    MyClass(const MyClass &obj) : value(obj.value)
    {
        cout << "Copy Constructor Called" << endl;
    }
    int getValue() { return value; }

private:
    int value;
};

MyClass func()
{
    MyClass obj(5);
    return obj;
}

void displayObject(MyClass obj)
{
    // Display code here
}

int main()
{
    // When an object of the class is returned by value
    MyClass newObj = func(); // Copy constructor called when object is returned by value

    // When an object of the class is passed (to a function) by value as an argument
    MyClass obj1(10);
    displayObject(obj1); // Copy constructor called when object is passed by value as an argument

    // When an object is constructed based on another object of the same class
    MyClass obj2 = obj1; // Copy constructor called when object is constructed based on another object

    // When the compiler generates a temporary object
    MyClass obj3 = MyClass(20); // Copy constructor called when a temporary object is generated

    return 0;
}