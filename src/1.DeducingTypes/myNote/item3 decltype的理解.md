## 理解`decltype`

给`decltype`一个变量名或表达式 它就会*精准*的告诉你结果(除了某些情况 ，在本文末尾介绍) ，而且它和`auto`和模板类型推导的规则不完全一样

### `auto`和`decltype`比较

decltype:

```cpp
const int i = 0;                //decltype(i)是const int 顶层const被保留

bool f(const Widget& w);        //decltype(w)是const Widget&
                                //decltype(f)是bool(const Widget&)

struct Point{
    int x,y;                    //decltype(Point::x)是int
};                              //decltype(Point::y)是int

Widget w;                       //decltype(w)是Widget

if (f(w))…                      //decltype(f(w))是bool

template<typename T>            //std::vector的简化版本
class vector{
public:
    …
    T& operator[](std::size_t index);
    …
};

vector<int> v;                  //decltype(v)是vector<int>
…
if (v[0] == 0)…                 //decltype(v[0])是int&

int arr[] = {1, 2, 3};
decltype(arr) arr_copy = arr;  // arr_copy的类型被推导为int[3]，即使arr没有被赋予一个变量
```

auto:

```cpp
int& ref = x;
auto ref_copy = ref;  // ref_copy的类型被推导为int，而不是int&

const int cx;
auto acx = cx; // 推导为int 顶层const被丢弃
```

最主要的区别就在于
`auto`推导时不保留引用 也不会保留 const ，
而`decltype`来了全保留了 ，确实很精准

### `decltype`最主要的用途就是用于声明函数模板，该函数的 ReturnType 依赖于形参类型
