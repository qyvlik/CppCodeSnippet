#ifndef QYVLIK_CALLABLE_H
#define QYVLIK_CALLABLE_H

#include "declara_forword.hpp"
#include "gc/gc.h"

namespace qyvlik {


class Callable : public gc::GCObject
{
protected:
    explicit Callable();
public:
    std::string name() const;

    void setName(const std::string& name);

    virtual ~Callable();

protected:
    virtual void exec(const Arguments& arguments, Value* result, Value* error) = 0;
private:
    std::string mName;
    friend class CallableCreator;

};

class CallableCreator
{
public:
    CallableCreator(gc::GarbageCollector* gc);
    virtual Callable* create(const std::string& name) = 0;
private:
    gc::GarbageCollector* mGC;
};



} // namespace qyvlik

#endif // QYVLIK_CALLABLE_H
