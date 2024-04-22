#include <iostream>
using namespace std;

class Vehicle
{
public:
    // virtual double get_weight() const = 0; // 版本1
    // virtual double get_weight() const { return 0.0; }; // 版本1改进（？）

    // 版本2 新增克隆函数
    virtual double get_weight() const = 0;
    virtual Vehicle *copy() const = 0;
    virtual ~Vehicle() {}
};
class RoadVehicle : public Vehicle
{
public:
    RoadVehicle(){};
    double get_weight() const { return 0.0; }

    // 版本2 新增克隆函数和拷贝构造
    Vehicle *copy() const
    {
        return new RoadVehicle(*this);
    }
    RoadVehicle(const RoadVehicle &other) {}

    auto getName() { return name; }
    ~RoadVehicle() {}

    int var{0};
    string name{"track"};
};
class AirCraft : public Vehicle
{
public:
    AirCraft(){};
    double get_weight() const { return 0.0; }

    Vehicle *copy() const
    {
        return new AirCraft(*this);
    }

    ~AirCraft() {}

    float score{1.0};
    string name{"plane"};
};

class VehicleSurrogate
{
public:
    VehicleSurrogate() : vp(nullptr) {}

    VehicleSurrogate(const VehicleSurrogate &vs)
    {
        if (this != &vs)
        {
            vp = vs.vp ? (vs.vp->copy()) : nullptr;
        }
    }

    VehicleSurrogate(const Vehicle &v) : vp(v.copy()) {}

    ~VehicleSurrogate() { delete vp; }

    // 代理Vehicle类行为的函数 Vehicle有多少个行为函数 这里就需要重新定义多少个代理类函数
    double get_weight() const
    {
        return vp->get_weight();
    }

private:
    Vehicle *vp;
};

int main(int argc, char const *argv[])
{
    // 版本1
    RoadVehicle arr1[100];
    AirCraft arr2[100];

    // Vehicle arr3[100]; // 想把这两个子类的实例都存在同一个数组里 但是这样写会报错
    // 原因为不能使用抽象类Vehicle
    // 假如把他改成虚函数 那就可以通过编译

    /* 版本1  需要使用是把坂本2新增代码注释 然后取消注释这段
    Vehicle arr4[100]; // 可以编译通过 那可以使用吗
    arr4[0] = RoadVehicle();
    arr4[1] = AirCraft();

    // cout << arr4[0].var << endl; // 报错！ Vehicle类里没有该成员属性
    // 说明版本1 的写法还是将子类的特有属性给裁剪了

    // 版本1改进2：用指针存储
    Vehicle *arr5[10];
    arr5[0] = new RoadVehicle();
    arr5[1] = new AirCraft();
    // cout << arr5[0]->get_weight() << endl; // 额 还不行 无法访问var name
    arr5[2] = new RoadVehicle();
    // cout << arr5[2]. // 评价为还不行
    */

    // 版本2  使用克隆函数 在调用arr[i]时调用一个克隆函数 返回一个自己的副本 然后返回副本的指针
    Vehicle *arr6[10];
    RoadVehicle *tmp = new RoadVehicle();
    arr6[0] = tmp;

    // cout << arr6[0]-

    // 版本3 使用代理类来管理
    VehicleSurrogate arr7[10];
    RoadVehicle x;
    arr7[0] = x;
    cout << arr7[0].get_weight() << endl;
    return 0;
}
