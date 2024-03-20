#include <iostream>
#include <vector>
using namespace std;

class Test
{
public:
    Test(int d1, int d2) { cout << "default" << endl; }
    Test(const Test &other) { cout << "copy" << endl; }
    Test(Test &&other) { cout << "move" << endl; }
};

int main(int argc, char const *argv[])
{
    vector<Test> myvec;
    cout << "看不懂思密达" << endl;

    Test tt(10, 20);
    cout << "================" << endl;
    // myvec.emplace_back(tt); // copy
    // myvec.emplace_back(Test(10, 20)); // move
    myvec.emplace_back(10, 20); // 直接构造进去了

    cout << "============" << endl;
    // myvec.push_back(tt); // copy*2
    // myvec.push_back(Test(10, 20)); // move copy
    myvec.push_back({19, 23}); // move copy

    //////////////////////////
    // emplace_back是编译慢
    vector<string> vec;
    vec.emplace_back("a");
    vec.emplace_back("aa");
    vec.emplace_back("aaa");

    // push_back是运行慢
    vec.push_back("a");
    vec.push_back("aa");
    vec.push_back("aaa");
    return 0;
}
