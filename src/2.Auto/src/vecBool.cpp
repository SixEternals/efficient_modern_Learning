#include <iostream>
#include <vector>
using namespace std;
int main(int argc, char const *argv[])
{
    // vector<bool> v{true, false, false};
    // bool *pb = &v[0];

    /*
    "std::_Bit_reference *" 类型的值不能用于初始化 "bool *" 类型的实体
    cannot convert 'std::vector<bool>::reference*' to 'bool*' in initialization
    taking address of rvalue [-fpermissive]
    */
    return 0;
}
