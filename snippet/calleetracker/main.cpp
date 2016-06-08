#include "calleetracker.hpp"

#define QYVLIK_CALLEE_PUSH_TRACK_ \
    qyvlik::CalleeTracker::DoDestory nil(__FILE__, __LINE__, __PRETTY_FUNCTION__); (void)nil;

#define QYVLIK_CALLEE_PRINT_TRACK \
    qyvlik::CalleeTracker::threadSingleton().printTrack();

void a();
void b();
void c();
void d();

int main()
{
    a();
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
    QYVLIK_CALLEE_PRINT_TRACK;
}
