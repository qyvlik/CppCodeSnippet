#ifndef QYVLIK_CALLABLE_H
#define QYVLIK_CALLABLE_H

#include "../declara_forword.hpp"
#include "../gc/gc.h"

namespace qyvlik{
namespace context {
class CallContext;

}
}

namespace qyvlik {
namespace domain {



class Callable : public gc::GCObject
{
protected:
    explicit Callable();
public:
    std::string name() const;

    void setName(const std::string& name);

    virtual ~Callable();
    virtual void call(context::CallContext* context);

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
    virtual Callable* create(const std::string& name);
private:
    gc::GarbageCollector* mGC;
};


} // namespace domain
} // namespace qyvlik

#endif // QYVLIK_CALLABLE_H
