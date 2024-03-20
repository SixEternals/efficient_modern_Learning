#include <iostream>
#include <functional>
#include <memory>
#include <vector>
using namespace std;
void func() { cout << "func" << endl; }

using LL = long long;
void func2(int arg1, LL arg2) { cout << arg1 << " " << arg2 << endl; }
class Myclass
{
    int a = 10;

public:
    static int class_func(int a, int b) { return a + b; }
    int operator()(int b) { return a + b; }
};
int gbl_var = 1000;

class captureThis
{
public:
    void print() { cout << "captureThis" << endl; }
    void test()
    {
        auto foo = [this]() // 这里只能写 [this]或者[&] 不能写[&this]
        { print(); };
        foo();
    }
};

function<void(void)> closureWrapper()
{
    int x = 10;
    return [&x]() mutable
    { cout << "value in the closure:" << x++ << endl; };
}

int main(int argc, char const *argv[])
{
    int a = 100, b = 99;
    // 1 捕获 不按[&]捕获的话 则其他类型的一捕获就初始化
    auto f1 = [&a, b1 = b]()
    { cout << b1 << endl;
        return a * 10; };
    cout << f1() << endl;

    a = 200, b = 9999;
    cout << f1() << endl;

    // 2 如上 lambda只是编译器的语法糖 它会帮助生成一个class 所以在main里和在lambda里算两个作用域
    // 如上图b是main里面的 而且它按值捕获 而b1属于lambda
    // 上面形式和    auto f1 = [&a, b = b]() 完全一致 等号左边的b不是右边的b

    // 2.1 除了列表{}以外 lambda表达式可以省略返回箭头
    auto neglect = [&]() -> string // 这里的 '-> string'可写可不写
    { return move("nihao"); };
    auto str = neglect();
    cout << str << endl;

    // auto NotNeglect = [&](int x) -> std::initializer_list<int> // 返回列表时不可省略
    // {
    //     initializer_list list = {12, 3, 4};
    //     return list; // 本身这样写也不行 返回了一个临时对象 initializer_list本来就只会创造临时对象
    // };
    // auto res = NotNeglect(1);
    auto NotNeglect2 = [&]() // 而相对于initializer_list 用vector等一系列容器会更安全 因为他们自己会申请内存
    {
        return move(vector{1, 2, 3, 4});
    };
    auto res2 = NotNeglect2();

    // 2.2 静态变量和局部变量不用捕获 可以直接使用
    static int cc = 888;
    auto ff = [&a]()
    {
        cout << a << cc << gbl_var << endl;
    };
    ff();

    // 2.3 lambda还能捕获this指针(用于类class里)
    captureThis obj;
    obj.test();

    // 2.4 lambda最有用的一集
    int myX = 10000000;
    auto copy_x = [&myX]
    { return myX * 2; };
    myX = 10;
    cout << "copy_x() = " << copy_x() << endl;

    // 3 闭包概念:包含上下文的函数(operator重载 类 lambda)
    // 涉及到function模板和bind函数
    // 3.1 function模板
    function<void(void)>
        f2 = func;
    f2();

    function<int(int, int)> f3 = Myclass::class_func; // 注意只能绑类的static函数

    Myclass c;
    function<int(int)> f4 = c;
    cout << f4(1) << endl;

    // 3.15 function和闭包的关系
    int factor{6};

    // f不是闭包 他是闭包的副本 类似于move 挺生内存的
    auto f11 = [&](int x, int y)
    { return factor * (x + y); };

    // 实际的闭包是个临时对象 通常在 句末 就被销毁 除非用
    auto &&rref = [&](int x, int y)
    { return factor * (x + y); };
    const auto &cref = [&](int x, int y)
    { return factor * (x + y); };

    int x = 10;
    auto func0 = [&x]()
    { x += 1, cout << "value in the closure:" << x << endl; };
    func0();

    // func1 不是闭包 而是包装闭包的function包装对象
    function<void(void)> func1 = closureWrapper();
    func1();

    // 3.2 bind函数 拥有占位符(使用时有专门的命名空间)
    // function<void(int, int)> f5 = bind(func, std::placeholders::_1, 2);
    function<void(int, int)> f5 = bind(func2, std::placeholders::_1, 2);
    f5(99, 99);
    bind(func2, std::placeholders::_1, std::placeholders::_2)(1000000, 10000000000); // 也可以这样写
    return 0;
}
