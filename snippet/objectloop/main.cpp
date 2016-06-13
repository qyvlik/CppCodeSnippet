#include <iostream>
#include <string>

#include "objectpool.hpp"

class MyObject : public qyvlik::Reusable
{
public:

    explicit MyObject():
        a(0)
    { }

    void print() const {
        std::cout << "a:" << a << std::endl;
    }

    void init() override
    {
        a = 1;
    }

    void clear() override
    {
        a = 0;
    }
private:
    int a;
};



int main()
{
    qyvlik::ObjectPool<MyObject> MyObjectPool;
    MyObjectPool.fillLoop();

    std::shared_ptr<MyObject> one = MyObjectPool.getOne();

    one->print();

    return 0;
}
