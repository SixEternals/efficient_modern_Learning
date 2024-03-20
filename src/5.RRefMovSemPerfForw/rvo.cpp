#include <iostream>
#include <vector>
#include <time.h>
using namespace std;

class Test
{
public:
    int a;
    Test() = default;
    Test(int a) : a(a) { cout << "Constructor" << endl; }
    Test(const Test &obj) : a(obj.a) { cout << "Copy" << endl; }
    Test(Test &&obj) { cout << "Move" << endl; }
    void print(int a) { cout << "In the member function PRINT" << a << endl; }
};

Test getTest()
{
    return Test(2);
}

void getTest(Test &_test)
{
    new (&_test) Test(2);
}

Test getTest2()
{
    Test t(3);
    return move(t);
}

void getTest2(Test &_test)
{
    // Test a(4);
    // new (&_test) Test(a); // 这样写仍有一次move

    // Test a(4);
    // new (&_test) Test(move(a)); // 这样写也会有一次move

    // Test a(4); // 优化器直接直到这个值是4
    new (&_test) Test(4);
}

Test func1()
{
    if (rand() % 2 == 0)
    {
        return Test(1);
    }
    else
    {
        Test t = Test(1);
        t.print(100);
        return t; // 1move
    }
}
void func1(Test &_test)
{
    if (rand() % 2 == 0)
    {
        cout << "1" << endl;
        new (&_test) Test(1);
    }
    else
    {
        cout << "0" << endl;
        new (&_test) Test(0);
        _test.print(100);
    }
}

Test func2(bool flag)
{
    Test t1(1);
    Test t2(0);
    if (flag)
    {
        return t1;
    }
    else
    {
        return t2;
    }
}

Test *func4()
{
    return new Test(2);
}

int main()
{
    // // 优化前
    // Test res = getTest(); // 切换至c17或更高时 就会强制优化 无论开没开'关闭优化'

    // // 编译器的优化类似于此
    // char buff[sizeof(Test)];
    // Test *ptr = reinterpret_cast<Test *>(buff);
    // getTest(*ptr);

    // &vector<int>(10, 1);

    // char buff[sizeof(Test)];
    // Test *ptr = reinterpret_cast<Test *>(buff);
    // getTest2(*ptr);

    // 失效的情况1 表达式或语句复杂
    // srand(time(nullptr));
    // // func1();
    // char buff[sizeof(Test)];
    // Test *ptr = reinterpret_cast<Test *>(buff);
    // func1(*ptr);

    // 失效情况2 有多个返回路径
    // func2(1);

    // 失败情况4 涉及动态分配
    func4();

    return 0;
}