#ifndef QYVLIK_CONTEXT_VIRTUALMACHINECONTEXT_H
#define QYVLIK_CONTEXT_VIRTUALMACHINECONTEXT_H

#include "../../declara_forword.hpp"

#include <stack>
#include <vector>
#include <cassert>

namespace qyvlik {
namespace context {

class EvaluationStack
{
public:
    EvaluationStack();

    ~EvaluationStack();

    void push(domain::Value* val);

    domain::Value* top() const;

    domain::Value* pop();

    size_t depth() const;

    size_t size() const;

    bool empty() const;

    void clear();

private:
    std::vector<domain::Value*> data;
};

class CallContext
{
public:
    explicit CallContext(virtual_machine::VirtualMachine* vm,
                         const domain::Arguments& arguments,
                         domain::Value* result,
                         domain::Value* error );

    size_t depth() const;

    gc::GarbageCollector* gc() const;

    domain::Arguments arguments() const;

    domain::Value* result() const;

    domain::Value* error() const;

    std::vector<const domain::Callable*> callers() const;

private:
    virtual_machine::VirtualMachine* mVM;
    domain::Arguments mArguments;
    domain::Value* mResult;
    domain::Value* mError;
};

class StackFrame
{
public:
    explicit StackFrame();
    explicit StackFrame(domain::Callable* callee);

    ~StackFrame();

    EvaluationStack* evalutaionStack();

    const EvaluationStack* evalutaionStack() const;

    void setCallee(domain::Callable* callee);

    domain::Callable* callee();

    int lastProgramCount() const;

    void setLastProgramCount(int pc);

    bool declaraVariable(int id, domain::Value* val);

    domain::Value* removeVariable(int id);

    domain::Value* variable(int id);


private:
    int mLastProgramCount;
    EvaluationStack* mEvaluationStack;
    domain::Callable* mCallee;
    std::map<int, domain::Value*> mVariableMap;
};

class VariableCreator
{
public:
    explicit VariableCreator(gc::GarbageCollector* gc);

    ~VariableCreator();

    domain::Value* createValue();

    domain::Value* createValue(bool val);

    domain::Value* createValue(int val);

    domain::Value* createValue(double val);

    domain::Value* createValue(const std::string& val);

    domain::Value* createValue(domain::Array* val);

    domain::Value* createValue(domain::Object* val);

    domain::Value* createValue(domain::Callable* val);

    domain::Array* createArray();

    domain::Object* createObject();

    gc::GarbageCollector* gc();

    const gc::GarbageCollector* gc() const;

    void destroy(gc::GCObject* o);

private:
    gc::GarbageCollector* mGC;

    domain::ArrayCreator* mArrayCreator;
    domain::ObjectCreator* mObjectCreator;
    //    domain::CallableCreator* callableCreator;
    domain::ValueCreator* mValueCreator;
};

// 计算栈
// 调用帧栈
class VirtualMachineContext
{
public:
    explicit VirtualMachineContext(gc::GarbageCollector* gc);

    ~VirtualMachineContext();

    StackFrame* rootStackFrame();

    const StackFrame* rootStackFrame() const;

    StackFrame* currentStackFrame();

    const StackFrame* currentStackFrame() const;

    void push(StackFrame* stackFrame);

    StackFrame* pop();

    size_t depth() const;

    VariableCreator* variableCreator();

    bool declaraVariable(StackFrame* stackFrame, int id, domain::Value *val);

    domain::Value* removeVariable(StackFrame* stackFrame, int id);

    domain::Value* variable(StackFrame* stackFrame, int id);

    gc::GarbageCollector* gc();

    const gc::GarbageCollector* gc() const;

    std::vector<StackFrame*>* stackFrameStack();

    const std::vector<StackFrame*>* stackFrameStack() const;

private:
    VariableCreator* mVariableCreator;
    std::vector<StackFrame*>* mStackFrameStack;
};

} // namespace context
} // namespace qyvlik

#endif // QYVLIK_CONTEXT_VIRTUALMACHINECONTEXT_H
