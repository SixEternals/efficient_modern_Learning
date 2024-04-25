#include <iostream>
#include <string.h>
#include <string>
#include <assert.h>
using namespace std;
class Animal
{
public:
    virtual void Eat() = 0;
    virtual Animal *copy() const = 0; // copy函数，构造一个基于自身对象类型的对象
    virtual ~Animal() {}
};
class Cat : public Animal
{
public:
    virtual void Eat()
    {
        std::cout << "cat eat." << std::endl;
    }
    virtual Animal *copy() const
    { // 返回一个以自身作为参数构造的Cat类型的对象
        return new Cat(*this);
    }
    ~Cat() {}
};
class Dog : public Animal
{
public:
    virtual void Eat()
    {
        std::cout << "dog eat." << std::endl;
    }
    virtual Animal *copy() const
    { // 返回一个以自身作为参数构造的Dog类型的对象
        return new Dog(*this);
    }
    ~Dog() {}
};
class Bird : public Animal
{
public:
    virtual void Eat()
    {
        std::cout << "bird eat." << std::endl;
    }
    virtual Animal *copy() const
    { // 返回一个以自身作为参数构造的Bird类型的对象
        return new Bird(*this);
    }
    ~Bird() {}
};
// 代理类
class AnimalSurrogate
{
public:
    AnimalSurrogate() : pa(NULL) {}

    // 拷贝一个管理对象的副本
    AnimalSurrogate(const Animal &ani)
    {
        pa = ani.copy();
    }

    // 拷贝构造
    AnimalSurrogate(const AnimalSurrogate &ani_srg)
    {
        pa = ani_srg.pa != nullptr ? ani_srg.pa->copy() : nullptr;
    }
    ~AnimalSurrogate()
    {
        if (pa != nullptr)
        {
            delete pa;
            pa = nullptr;
        }
    } // 重载 = 操作符

    AnimalSurrogate &operator=(const AnimalSurrogate &ani_srg)
    {
        if (this != &ani_srg)
        {
            delete pa;
            pa = ani_srg.pa != nullptr ? ani_srg.pa->copy() : nullptr;
        }
        return *this;
    }

    // 将基类中的公共函数搬过来,这样就可以通过代理类直接访问这些方法
    void Eat()
    {
        if (pa == nullptr)
        {
            throw "empty AnimalSurrogate.Eat()";
        }
        return pa->Eat();
    }

private:
    Animal *pa; // 存储基类的指针};
};

// ===================================
// 句柄类示例
// ===================================
class MyString
{
public:
    MyString()
    {
        cout << "MyString()" << endl;
        _buf = new char[1];
        _buf[0] = '\0';
        _len = 0;
    }

    // const char*参数的构造函数
    MyString(const char *str)
    {
        cout << "MyString(const char *str)" << endl;

        if (str == nullptr)
        {
            _len = 0, _buf = new char[1];
            _buf[0] = '\0';
        }
        else
        {
            _len = strlen(str);
            _buf = new char[_len + 1];
            strcpy_s(_buf, _len + 1, str);
        }
    }
    // 拷贝偶早函数
    MyString(const MyString &other)
    {
        cout << "MyString(const MyString& other)" << endl;
        _len = strlen(other._buf);
        _buf = new char[_len + 1];
        strcpy_s(_buf, _len + 1, other._buf);
    }

    // 重载赋值运算符
    const MyString &operator=(const MyString &other)
    { // 要判定是不是同一个对象
        if (this != &other)
        {
            if (other._len > this->_len)
            {
                delete[] _buf;
                _buf = new char[other._len + 1];
            }

            _len = other._len;
            strcpy_s(_buf, _len + 1, other._buf);
        }
        return *this;
    }

    // str = "hello" 重载等号
    const MyString &operator=(const char *str)
    {
        assert(str != nullptr);
        cout << "const MyString &operator=(const char *str)" << endl;

        size_t StrLen = strlen(str);
        if (StrLen > _len)
        {
            delete[] _buf;
            _buf = new char[StrLen + 1];
        }
        _len = StrLen;
        strcpy_s(_buf, StrLen + 1, str);
        return *this;
    }

    // str += "hello"
    const MyString &operator+=(const char *str)
    {
        assert(str != nullptr);
        cout << "const MyString &operator+=(const char *str)" << endl;
        if (str == nullptr)
            return *this;

        size_t StrLen = strlen(str);
        size_t newSize = StrLen + 1 + _len;
        char *newBuff = new char[newSize];
        // https://cplusplus.com/reference/cstring/strcat/   strcat用法
        strcpy_s(newBuff, newSize, _buf);
        strcat_s(newBuff, newSize, str);

        delete[] _buf;
        _buf = newBuff;
        _len = newSize;

        return *this;
    }

    char *getBuf() { return _buf; }

    friend std::ostream &operator<<(std::ostream &out, const MyString &str)
    {
        out << str._buf;
        return out;
    }
    const size_t length() { return _len; }

    ~MyString()
    {
        cout << "~MyString()" << endl;
        delete[] _buf;
        _buf = nullptr;
        _len = 0;
    }

private:
    char *_buf;
    size_t _len;
};

// 引入计数类 用于MyStringProxy
class RefCount
{
public:
    RefCount() : _count(new int(1)) {}
    ~RefCount() = default;

    void addRef() { ++*_count; }
    bool releaseRef() { return --*_count == 0; }
    bool isUnique() const { return *_count == 1; }

private:
    int *_count;
};

// MyString 代理类
class MyStringProxy
{
public:
    MyStringProxy() : _ptr(new MyString()), _refCount(new RefCount()) {}

    MyStringProxy(const MyString &other) : _ptr(new MyString(other)), _refCount(new RefCount()) {}

    MyStringProxy(const MyStringProxy &other)
        : _ptr(other._ptr), _refCount(other._refCount)
    {
        _refCount->addRef();
    }

    ~MyStringProxy()
    {
        if (_refCount->releaseRef())
        {
            delete _ptr;
            delete _refCount;
        }
    }

private:
    MyString *_ptr;
    RefCount *_refCount; // 计数类
};

int main(int argc, char const *argv[])
{
    Cat cat;
    Dog dog;
    Bird bird;

    AnimalSurrogate arr[10];
    arr[0] = AnimalSurrogate(cat);
    arr[1] = AnimalSurrogate(dog);
    arr[2] = AnimalSurrogate(bird);

    // arr[0].Eat();
    // arr[1].Eat();
    // arr[2].Eat();
    // 上面就是代理类的示例

    // 因为每个代理类都会创建一个新的对象 存在很多不必要的拷贝 可以使用句柄类来优化
    // 句柄类
    MyString obj1("Hello");
    MyString obj2(" World!");
    cout << obj1 << endl;

    obj1 += " world!";
    cout << obj1 << endl;

    obj1 = obj2;
    cout << obj1 << endl;

    return 0;
}
