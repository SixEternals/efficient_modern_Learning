#include <iostream>
using namespace std;
// int g(int i)
// {
//     return 10;
// }
// std::string g(int i)
// {
//     return "hello";
// }
template <int i>
auto f()
{
    if constexpr (i == 0)
    {
        return 10;
    }
    else if constexpr (i == 1)
    {
        return "nihao";
    }
}
int main(int argc, char const *argv[])
{
    cout << f<0>() << endl;
    cout << f<1>() << endl;

    return 0;
}
