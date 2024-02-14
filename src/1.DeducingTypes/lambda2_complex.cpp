#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    auto glambda = [](auto a, auto &&b)
    { return a < b; };
    bool b = glambda(3, 3.15); // return true

    auto vglambda = [](auto printer)
    {
        return [=](auto &&...ts)
        {
            printer(std::forward<decltype(ts)>(ts)...); // 被转发为右值引用
            return [=]
            { printer(ts...); };
        };
    };

    auto p = vglambda([](auto v1, auto v2, auto v3)
                      { std::cout << v1 << " " << v2 << " " << v3; });

    auto q = p(1, 'a', 3.14); // 传入三个右值
    q();
    return 0;
}
