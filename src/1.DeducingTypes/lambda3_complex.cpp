#include <iostream>
#include <vector>

int main()
{
    // 声明一个lambda函数vglambda，接受一个函数参数modifier
    auto vglambda = [](auto modifier)
    {
        // 返回一个新的lambda函数
        return [=](auto &&vec) // 通用引用
        {
            // 在新lambda函数中调用传递进来的modifier函数，修改vec
            modifier(vec);

            // 返回修改后的vec
            return vec;
        };
    };

    // 使用vglambda定义一个新的lambda函数p
    auto p = vglambda([](auto &&vec)
                      {
                         // 修改vec，将所有元素都乘以2
                         for (auto &elem : vec)
                         {
                             elem *= 2;
                         } });

    // 调用lambda函数p，并传递一个初始的vector
    std::vector<int> vec{1, 2, 3, 4, 5};
    auto modifiedVec = p(vec);

    // 打印修改后的vector中的元素
    for (const auto &elem : modifiedVec)
    {
        std::cout << elem << " ";
    }

    return 0;
}