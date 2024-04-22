#include <iostream>
using namespace std;
class test
{
public:
    explicit test() noexcept {}
    explicit test(const int val) noexcept : val(val) { cout << "default" << endl; }

    test(const test &other) noexcept : val(other.val)
    {
        cout << "copied" << endl;
    }

    test &operator&=(const test &other) noexcept
    {
        this->val = other.val;
        cout << "equals" << endl;
        return *this;
    }

private:
    int val;
};

int main(int argc, char const *argv[])
{
    test t1(10);
    test t2 = test(t1); // 显式地使用拷贝构造函数

    test t3 = t1; // 隐式地调用拷贝构造函数 如果拷贝构造函数被explict标记的话 那这句就会报错

    // test t3 = 10;// 隐式的调用构造函数 如果构造函数被标记explict的话这句也会报错

    /*
    ∴对各种构造函数使用explict时 要考虑清楚要不要舍弃隐式转换的写法
    */
    return 0;
}
