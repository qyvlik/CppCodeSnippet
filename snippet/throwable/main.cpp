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
    QYVLIK_CALLEE_PUSH_TRACK;
    b();
}

void b() {
    QYVLIK_CALLEE_PUSH_TRACK;
    c();
}

void c() {
    QYVLIK_CALLEE_PUSH_TRACK;
    d();
}

void d() {
    QYVLIK_CALLEE_PUSH_TRACK;
    throw qyvlik::Throwable(0, "Throwalbe ");
}
