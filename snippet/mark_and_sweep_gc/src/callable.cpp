#include "callable.h"

namespace qyvlik {


Callable::Callable():
    GCObject()
{}

std::string Callable::name() const {
    return mName;
}

void Callable::setName(const std::string &name) {
    this->mName = name;
}

Callable::~Callable()
{
    std::cout << "~Callable: [" << this << "]" << std::endl;
}

//---------------------------------------------------------

CallableCreator::CallableCreator(gc::GarbageCollector *gc):
    mGC(gc)
{ }

} // namespace qyvlik

