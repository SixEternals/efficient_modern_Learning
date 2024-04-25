#include <iostream>
#include <string.h>
#include <vector>
#include <array>
#include <memory>
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

void func1() // 实验1
{
    student s(1001, "Manjeet", 10000);
    // s.display();

    student manjeet(s); // copy constructor called
    // manjeet.display();

    A a = 10;
    A b(10);
    A c(a);
    A d = a;

    test();
}

struct point
{
    int x, y;
};

void func2() // 实验2
{
    std::array<point, 3> x1{1, 2, 3, 4, 5, 6}; // √
    // std::array<point, 3> x2{{1, 2}, {3, 4}, {4, 5}}; // 报错！
    std::array<point, 3> x2{{{1, 2}, {3, 4}, {4, 5}}}; // √
}
class testClass
{
public:
    testClass() {}
    int var{0};
};

void func3() // 实验3
{
    // testClass t(); // 识别为函数声明
    testClass t2{};
    cout << t2.var << endl;
}

// 实验4
template <typename T>
void print(const T &obj)
{
}
// 假设 T 是我们要创建的对象类型，而 Args... 是传递给构造函数的参数包。
template <typename T, typename... Args>
void createObject(Args &&...args)
{
    // 使用圆括号构造 对象 ，适用于接收多个参数的构造函数。
    T localObjectWithParens(std::forward<Args>(args)...);

    // 使用花括号构造对象，适用于接收一个参数（通常是 std::initializer_list 或聚合类型）的构造函数。
    T localObjectWithBraces{std::forward<Args>(args)...};

    // 打印 localObjectWithParens 和 localObjectWithBraces 的内容，以展示它们是如何被构造的。
    // 这里我们使用了一个简单的打印函数，它需要为 T 提供。
    print(localObjectWithParens); // 假设 print 能够处理 T 类型的对象。
    print(localObjectWithBraces);
}
void func4() // 实验4
{
    createObject<std::vector<int>>(10, 20);
}

// 5
// 假设这是我们的模板函数，它接受任意数量的参数来创建一个对象
template <typename T, typename... Args>
void doSomeWork(Args &&...args)
{
    // 使用圆括号创建对象
    T localObjectWithParens(std::forward<Args>(args)...);
    // 使用花括号创建对象
    T localObjectWithBraces{std::forward<Args>(args)...};

    // 为了演示，我们简单地打印出 localObjectWithParens 的大小
    // 对于 std::vector，这将展示它包含的元素数量
    if constexpr (std::is_same_v<T, std::vector<int>>)
    {
        std::cout << "Size with parens: " << localObjectWithParens.size() << '\n';
        std::cout << "Size with braces: " << localObjectWithBraces.size() << '\n';
    }
}
// 重载版本，用于接收 std::initializer_list
template <typename T>
void doSomeWork(std::initializer_list<typename T::value_type> initList)
{
    // 使用初始化列表创建对象
    T localObjectWithBraces(initList);

    // 为了演示，我们简单地打印出 localObjectWithBraces 的大小
    // 对于 std::vector，这将展示它包含的元素数量
    if constexpr (std::is_same_v<T, std::vector<int>>)
    {
        std::cout << "Size with braces: " << localObjectWithBraces.size() << '\n';
    }
}
void func5()
{
    // 调用 doSomeWork 来创建一个包含两个整数值的 std::vector<int>
    doSomeWork<std::vector<int>>(10, 20);

    // 调用 doSomeWork 来创建一个包含单个整数数组的 std::vector<int>
    doSomeWork<std::vector<int>>({10, 20}); // 需要知道的是

    // 可以使用 std::make_unique 和 std::make_shared 来演示，但它们不在示例范围内
}

int main()
{
    // func1(); // 实验1

    // func2(); // 实验2

    func4();
    return 0;
}
