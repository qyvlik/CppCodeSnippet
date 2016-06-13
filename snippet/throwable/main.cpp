#include <iostream>

#include "src/throwable.hpp"

void a();
void b();
void c();
void d();

int main()
{
    try {
        a();
    } catch(qyvlik::Throwable e) {
        e.printTrack();
    }
    return 0;
}

void a() {
    QYVLIK_CALLEE_PUSH_TRACK_;
    b();
}

void b() {
    QYVLIK_CALLEE_PUSH_TRACK_;
    c();
}

void c() {
    QYVLIK_CALLEE_PUSH_TRACK_;
    d();
}

void d() {
    QYVLIK_CALLEE_PUSH_TRACK_;
    throw qyvlik::Throwable(0, "Throwalbe ");
}
