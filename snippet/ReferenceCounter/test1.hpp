#ifndef TEST1
#define TEST1

#include <iostream>
#include "src/reference.hpp"

using namespace qyvlik;

class Object
{
public:
    virtual ~Object()
    {}
    virtual void print() const {
        std::cout << "Object[" << this << "]" << std::endl;
    }
};

class ObjectA : public Object
{
public:
    ObjectA() = default;
    virtual void print() const {
        std::cout << "ObjectA[" << this << "]" << std::endl;
    }
};

void test1()
{
    ObjectCreator<ObjectA> creator;

    Reference<ObjectA> a;
    {
        a = creator.create();
        auto b = a.cast<Object>();
        b->print();
        creator.destroy(a);
    }
    std::cout << "pointer: " << a.pointer() << std::endl;   // nullptr
    // a->print();
}

#endif // TEST1

