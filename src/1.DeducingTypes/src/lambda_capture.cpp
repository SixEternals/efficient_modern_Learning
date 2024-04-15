#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char const *argv[])
{
    // TAG:实验1
    int x = 27;
    int &rx = x;
    int *px = &x;
    const int cx = x;
    const int &crx = x;

    cout << "the first func: auto f = [&](){ coding... }";
    auto f = [&]()
    {
        cout << x << " " << rx << " " << *px << " " << cx << " " << crx << endl;
    };
    f();

    x = 100;
    cout << "check:";
    f();
    x = 27;

    cout << "the second func: auto f = [&](int x,int rx){ coding... }";
    decltype(auto) f1 = [](int x, int &rx, int *px, const int cx, const int &crx)
    {
        x = 300;
        cout << x << " " << rx << " " << *px << " " << cx << " " << crx << endl;
    };
    f1(x, rx, px, cx, crx);

    cout << "the third is check:";
    f();

    // TAG: 第二份实验
    vector<int> arr = {1, 4, 2, 7, 10, -1};
    cout << "\n\noriginal arr address:" << &arr << endl;

    cout << "arr address , pass by value:" << endl;
    decltype(auto) f2 = [=](auto arr)
    {
        cout << "arr address inside f2:" << &arr << endl;
    };
    f2(arr);

    cout << "\narr address , pass by refenrce:" << endl;
    decltype(auto) f3 = [&](auto &arr)
    {
        cout << "arr address inside f3:" << &arr << endl;
    };
    f3(arr);
    return 0;
}
