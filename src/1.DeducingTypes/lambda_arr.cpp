#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main(int argc, char const *argv[])
{
    /* code */
    // TAG:实验1
    auto glambda = [](auto a, auto &&b)
    { return a < b; };
    bool b = glambda(3, 3.14);
    cout << "b = " << b << endl;

    // TAG:实验2
    vector<int> arr = {5, 2, 5, 4, 3, 6, 10, 1};
    sort(arr.begin(), arr.end(), [](int a, int b)
         { return a < b; });

    cout << "arr address in the outside main:" << &arr << endl;

    auto print = [](vector<int> &arr)
    {
        cout << "arr address in the inside print function:" << &arr << endl;
        for (auto i : arr)
        {
            cout << i << " ";
        }
    };
    print(arr);
    return 0;
}
