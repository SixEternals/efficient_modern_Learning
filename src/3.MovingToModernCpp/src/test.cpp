#include <iostream>

constexpr auto GetDimensions(double boxThickness,
                             double ballDiameter)
{

    if (boxThickness > 2 * ballDiameter)
        return 3;
    else if (boxThickness > ballDiameter)
        return 2;
    else
        return 0;
}

template <int size>
struct ball
{
    int location[size] = {};
};

template <int size>
auto print(ball<size> &p)
{
    for (int i = 0; i < size; i++)
        std::cout << p.location[i];
}

// Change box thickness to 5, 15 and 30
// which leads to error, 2D and 3D compilation
constexpr double boxThickness = 30;
constexpr double ballDiameter = 10;

constexpr int dim = GetDimensions(boxThickness, ballDiameter);
static_assert(dim, "Dim must be > 0");

int main()
{

    ball<dim> p;

    print(p); // 00 for 2D and 000 for 3D

    return 0;
}