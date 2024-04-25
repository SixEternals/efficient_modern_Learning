#include <iostream>
#include <initializer_list>
using namespace std;

/*
一种我们常见的可变参数的使用实在模板中的可变参数包
在C++中 `...`表示可以接受可变数目的参数 在C++中称为参数包
通过利用 模板 参数包 递归 我们可以实现对不同类型 不定长度的入参处理
*/
template <typename T>
void printf_(const T &input)
{
    std::cout << input << std::endl;
}
template <typename T, typename... Args>
void printf_(const T &num, const Args &...rest)
{
    std::cout << num << std::endl;
    printf_(rest...);
}

template <typename T>
void printf_(std::initializer_list<T> args)
{
    for (auto i : args)
    {
        cout << i << endl;
    }
}

int main()
{
    // 实验1
    // printf_("s", 2, 3, "HELLO"); // 依次输出s,2,3, HELLO

    // 实验2
    std::initializer_list<int> x{1, 2, 3, 4, 4};
    printf_(x);
    return 0;
}