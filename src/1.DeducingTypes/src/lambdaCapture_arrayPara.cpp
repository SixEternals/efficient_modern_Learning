#include <iostream>
#include <array>
using namespace std;
template <typename T, std::size_t N>               // 关于
constexpr std::size_t arraySize(T (&)[N]) noexcept // constexpr
{                                                  // 和noexcept
    return N;                                      // 的信息
}

void f(const int arr[])
{
    // 在函数声明中 就算声明为数组arr[] 但是还是会视为指针 即f()重载的两种写法都一样
}
// TAG:void f(const int *arr) 而且这段被视为了“重定义”

// 所以 真想用到数组引用传入的话应该这样写 但是 这个只能告诉编译器数组长度 而不能知道数组本体 可以将数组降为指针使用
template <typename T, size_t N>
constexpr void func(T (&arr)[N]) noexcept;

int main()
{
    // expr1
    int keyVals[] = {1, 3, 7, 9, 11, 22, 35}; // keyVals有七个元素
    cout << "now in the main:" << &keyVals << endl;

    // int mappedVals[arraySize(keyVals)];
    std::array<int, arraySize(keyVals)> mappedVals;
    func(keyVals);

    // expe2
    const char a1[] = "hello , ell ri onn";
    const char *a2 = a1; // 数组“降级”为指针

    return 0;
}
template <typename T, size_t N>
constexpr void func(T (&arr)[N]) noexcept
{
    cout << "now in the func:" << &arr << endl;
    for (int i = 0; i < N; ++i)
        cout << arr[i] << endl;
}