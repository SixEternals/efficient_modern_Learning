#include <iostream>

using namespace std;

template <typename T>
void f(T &param)
{
    cout << param[0] << endl;
}
template <typename T>
void f2(T param)
{
    cout << 1 << endl;
}
template <typename T, size_t N>
constexpr size_t f3(T (&)[N]) noexcept
{
    return N;
}
int main()
{
    int arr[19]{
        1,
        23,
        4,
        1,
        2,
        24,
        2,
    };
    f(arr);
    f2(arr);

    // ------------------

    return 0;
}