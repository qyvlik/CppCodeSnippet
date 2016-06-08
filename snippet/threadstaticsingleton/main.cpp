#include <iostream>
#include <thread>

#include "threadstaticsingleton.hpp"

class Object {
public:
    void print() const {
        std::cout << std::this_thread::get_id() << ": Object(" << this << ")" << std::endl;
    }
};

int main()
{

    std::thread([&](){
        qyvlik::ThreadStaticSingleton<Object>::getThreadstaticSingleton().print();
    }).join();

    std::thread([&](){
        qyvlik::ThreadStaticSingleton<Object>::getThreadstaticSingleton().print();
    }).join();

    std::thread([&](){
        qyvlik::ThreadStaticSingleton<Object>::getThreadstaticSingleton().print();
    }).join();
}
