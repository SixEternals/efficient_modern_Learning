#include <iostream>
using namespace std;

void print(unsigned int u) // note: unsigned
{
    std::cout << u << '\n';
}
class Widget
{
public:
    Widget(int i, bool b);                  // 同之前一样
    Widget(int i, double d);                // 同之前一样
    Widget(std::initializer_list<bool> il); // 现在元素类型为bool
                                            // 没有隐式转换函数
};

int main()
{
    // TAG:item7实验1
    std::cout << "Enter an integral value: ";
    int n{};
    std::cin >> n; // enter 5 or -5
    print(n);      // conversion to unsigned may or may not preserve value

    // TAG:item7实验2
    constexpr int n1{5}; // note: constexpr
    unsigned int u1{n1}; // okay: conversion is not narrowing due to exclusion clause

    constexpr int n2{-5}; // note: constexpr
    // unsigned int u2{n2};  // compile error: conversion is narrowing due to value change 这句话会报错

    // 3
    // Widget w{10, 5.0}; // 错误！要求变窄转换

    return 0;
}
