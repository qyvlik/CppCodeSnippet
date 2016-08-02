#ifndef TEST1
#define TEST1

#include <iostream>
#include "src/reference.hpp"

using namespace qyvlik;

class Object
{
public:
    void print() const {
        std::cout << "Object[" << this << "]" << std::endl;
    }
};


void test1()
{
    ObjectCreator<Object> creator;

    Reference<Object> a;
    {
        a = creator.create();
        a->print();
        creator.destroy(a);
    }
    std::cout << "pointer: " << a.pointer() << std::endl;   // nullptr
    // a->print();
}

#endif // TEST1

