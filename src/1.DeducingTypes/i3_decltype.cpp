#include <iostream>
#include <assert.h>
using namespace std;

// TAG:decltype实验1
struct A
{
    double x;
};
const A *a;
decltype(a->x) y; // y为double
// decltype((a->x)) z; // z为double& 左值引用 多余的括号很神奇吧

// TAG:decltype实验2
const int &getRef(const int *p) { return *p; }
static_assert(std::is_same<decltype(getRef), const int &(const int *)>::value); // 通过 说明getRef的类型是const int&(const int*)

auto getRefFwdBad(const int *p) { return getRef(p); }
static_assert(std::is_same<decltype(getRefFwdBad), int(const int *)>::value,
              "Just returning auto isn't perfect forwarding."); // 通过 如上一节auto所说 虽然getRef返回一个左值 但是用的是auto返回 所以返回成int了

decltype(auto) getRefFwdGood(const int *p) { return getRef(p); }
static_assert(std::is_same<decltype(getRefFwdGood), const int &(const int *)>::value,
              "Returning decltype(auto) perfectly forwards the return type."); // 通过 说明使用decltype(auto)才能返回正确的 想要返回的类型：const int &

int main(int argc, char const *argv[])
{
    // TAG:is_same 前置知识
    // cout << is_same<int, int>::value << endl;    // true
    // cout << is_same<int, double>::value << endl; // false
    // cout << is_same<int, double>::value_type << endl; // invalid 类型名无法打印

    // TAG:decltype实验3
    int i = 33;
    decltype(i) j = i * 2;
    static_assert(std::is_same<decltype(i), decltype(j)>::value); // true
    assert(i == 33 && 66 == j);

    auto f = [i](int av, int bv) -> int
    { return av * bv + i; };
    auto h = [i](int av, int bv) -> int
    { return av * bv + i; };
    static_assert(!std::is_same<decltype(f), decltype(h)>::value,
                  "The type of a lambda function is unique and unnamed"); // true 因此不会打印这条消息

    decltype(f) g = f;
    std::cout << f(3, 3) << ' ' << g(3, 3) << '\n';
    return 0;
}
