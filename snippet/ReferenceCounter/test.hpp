#ifndef TEST_HPP
#define TEST_HPP
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <vector>
#include <system_error>

#include "src/referecnecountpointer.hpp"

using namespace qyvlik;


class MyClass
{
public:
    virtual ~MyClass(){
        std::cout << "~MyClass() [" << this << "]" << std::endl;
    }
    virtual void print() const {
        std::cout << "print :[" << this << "]" << std::endl;
    }
};

class MyClass0 : public MyClass
{
public:
    virtual ~MyClass0(){
        std::cout << "~MyClass0 [" << this << "]" << std::endl;
    }
    virtual void print() const {
        std::cout << "MyClass0::print :[" << this << "]" << std::endl;
    }
};


ReferecneCountPointer<MyClass> shared(new MyClass0());

void get() {

    std::vector< ReferecneCountPointer<MyClass> > get_s;

    int i = 64;
    while(i > 0) {

        get_s.push_back(shared);

        i--;
    }

    //std::cout << "use count: " << shared.useCount() << std::endl;
    //std::cout << "Waiting for the 5 seconds to begin...\n";

    std::this_thread::sleep_for (std::chrono::seconds(5));
}

void test()
{
    int i = 872;            // 872 Resource temporarily unavailable RtlWerpReportException
    std::vector<std::thread> threads;

    try {
        while(i > 0) {
            threads.push_back(std::thread(get));
            i--;
        }
    } catch(std::system_error e) {
        std::cout << e.code() << std::endl;
        std::cout << e.what() << std::endl;
        std::cout << "thread count is :" << 872-i << std::endl;
    }

    std::for_each(threads.begin(), threads.end(), [](std::thread& t){
        t.join();
    });

    if(shared.cast<MyClass0>()) {
        shared.cast<MyClass0>()->print();
    }

    // while(true);
}



#endif // TEST_HPP

