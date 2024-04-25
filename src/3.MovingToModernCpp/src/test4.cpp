#include <iostream>
#include <tuple>

// c++11开始使用
using namespace std;
int main(int argc, char const *argv[])
{
    std::tuple<int, string, double> worker;
    // worker = make_tuple(1, "wang", 12.7);
    // cout << get<0>(worker) << endl;
    // cout << get<1>(worker) << endl;
    // cout << get<2>(worker) << endl;
    // for (int i = 0; i < 3; ++i)
    //     cout << get<i>(worker) << endl;

    enum color1
    {
        red,
        yellow
    };
    // auto yellow = 100; // error
    enum class color2
    {
        blur,
        green
    };
    auto green = 100; 

    return 0;
}
