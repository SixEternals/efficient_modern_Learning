#include <iostream>
using namespace std;
template <typename T>
void myFunction(T list)
{
    // ...
}

template <typename T>
void myFunction(std::initializer_list<T> list)
{
    // ...
}

int main(int argc, char const *argv[])
{
    // // auto的推导大部分都和条款1的推导差不多

    // // TAG：对数组使用auto
    // const char name[] = "hello guys";

    // auto arr1 = name; // 降级为指针
    // auto &arr2 = name;
    // auto &&arr3 = name; // 左值引用

    // // TAG：对函数使用引用
    // void someFunc(int, double);
    // auto func1 = someFunc;  // 降级为指针 func1的类型为void (*)(int, double)
    // auto &func2 = someFunc; // func2的类型为void (&)(int, double)

    // // TAG：不同点：对于initallizer_list
    // myFunction(1);
    // myFunction({1});
    // //    myFunction{1}; // 不能这样写
    // myFunction({1, 2, 3}); // T 被推导为 int

    cout << "test" << endl;
    return 0;
}
