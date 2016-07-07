#include "callable.h"
#include "../engine/context/virtualmachinecontext.h"
#include "../domain/value.h"

namespace qyvlik {
namespace domain {

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

void Callable::call(context::CallContext *context)
{
    this->exec(context->arguments(), context->result(), context->error());
}

//---------------------------------------------------------

class PrintCallee : public Callable
{
public:
    PrintCallee():
        Callable()
    {
        setName("print");
    }
protected:
    void exec(const std::vector<domain::Value *> &arguments, domain::Value *result, domain::Value *error) override
    {
        //std::cout << "-------------------------print start----------------------" << std::endl;

        (void)error;
        auto iter = arguments.begin();
        auto end  = arguments.end();
        while(iter != end) {

            std::cout << (*iter)->toString() << " ";

            iter++;
        }
        std::cout << std::endl;
        if(result) {
            result->assign((int)arguments.size());
        }
        //std::cout << "-------------------------print  end----------------------" << std::endl;
    }
};

class TrackCallee : public Callable
{
public:
    TrackCallee():
        Callable()
    {
        this->setName("track");
    }

public:
    void call(context::CallContext *context) override
    {
        auto callers = context->callers();
        auto iter = callers.begin();
        auto end  = callers.end();
        while(iter != end) {

            std::cout << "\t" << (*iter)->name() << std::endl;

            iter++;
        }
    }

protected:
    void exec(const std::vector<domain::Value *> &arguments, domain::Value *result, domain::Value *error) override
    {
        (void)arguments;
        (void)result;
        (void)error;
    }
};

CallableCreator::CallableCreator(gc::GarbageCollector *gc):
    mGC(gc)
{}

Callable *CallableCreator::create(const std::string &name)
{
    Callable* callee;
    if(name == "print") {
        callee = new PrintCallee();
        mGC->addObject(callee);
        return callee;
    } else if(name == "track") {
        callee = new TrackCallee();
        mGC->addObject(callee);
        return callee;
    } else {
        return nullptr;
    }
}

//---------------------------------------------------------------------

} // namespace domain
} // namespace qyvlik

