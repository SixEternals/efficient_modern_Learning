#include <iostream>
using namespace std;
// ===========================================
// 在proxy2中 仍然有一些问题
// 1. 多个指针指向同一块动态内存，内存改何时释放？由谁释放？
// 2. 如果某个对象需要修改字符串中的内容，该如和处理？
//
// 解决这些问题，在C++中有两个比较经典的方案，那就是引用计数和Copy On Write。
// ===========================================

// 下面给出引用计数的一个封装类：
class RefCount
{
public:
    RefCount() : _count(new int(1)) {} // 初始引用计数设置为1

    RefCount(const RefCount &other) : _count(other._count)
    {
        ++*_count; // 拷贝后 计数加1
    }

    ~RefCount()
    {
        if (--*_count == 0)
        {
            delete _count;
            _count = nullptr;
        }
    }

    bool Only()
    {
        return *_count == 1;
    }

    void ReAttach(const RefCount &other)
    { // 重新绑定引用计数
        if (Only())
        {
            // 如果当前对象是唯一的引用，则删除当前的计数器
            delete _count;
        }
        else
        {
            --*_count;
        }

        // 更新新的引用计数的信息
        ++*other._count;

        // 绑定到新的引用计数
        _count = other._count;
    }

    void MakeNewRef()
    {
        if (*_count > 1)
        {
            --*_count;
            _count = new int(1);
        }
    }

private:
    int *_count;
};
int main(int argc, char const *argv[])
{

    return 0;
}
