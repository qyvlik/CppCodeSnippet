#include "virtualmachinecontext.h"
#include "../virtual_machine/virtualmachine.h"

#include "../../domain/value.h"
#include "../../domain/array.h"
#include "../../domain/callable.h"
#include "../../domain/object.h"


namespace qyvlik {
namespace context {

EvaluationStack::EvaluationStack()
{}

EvaluationStack::~EvaluationStack()
{}

void EvaluationStack::push(domain::Value *val)
{
    data.push_back(val);
}

domain::Value *EvaluationStack::top() const
{
    return data.back();
}

domain::Value *EvaluationStack::pop()
{
    if(!data.empty()) {
        domain::Value* popValue = data.back();
        data.pop_back();
        return popValue;
    } else {
        return nullptr;
    }
}

size_t EvaluationStack::depth() const
{
    return data.size();
}

size_t EvaluationStack::size() const
{
    return data.size();
}

bool EvaluationStack::empty() const
{
    return data.empty();
}

void EvaluationStack::clear()
{
    data.clear();
}


//--------------------------------------------------------------------------

CallContext::CallContext(virtual_machine::VirtualMachine *vm,
                         const domain::Arguments &arguments,
                         domain::Value *result,
                         domain::Value *error):
    mVM(vm),
    mArguments(arguments),
    mResult(result),
    mError(error)
{ }

size_t CallContext::depth() const
{
    return mVM->stackFrameStackDepth();
}

domain::Arguments CallContext::arguments() const {
    return mArguments;
}

gc::GarbageCollector *CallContext::gc() const {
    return mVM->gc();
}

domain::Value *CallContext::result() const {
    return mResult;
}

domain::Value *CallContext::error() const {
    return mError;
}

// back is top
std::vector<const domain::Callable *> CallContext::callers() const
{
    std::vector<const domain::Callable *> caller_vector;
    const std::vector<context::StackFrame*>* call_stack = this->mVM->stackFrameStack();
    auto iter = call_stack->begin();
    auto end  = call_stack->end();
    while(iter != end) {

        caller_vector.push_back((*iter)->callee());

        iter++;
    }
    return caller_vector;
}

//--------------------------------------------------------------------------

StackFrame::StackFrame():
    mLastProgramCount(-1),
    mEvaluationStack(new EvaluationStack()),
    mCallee(nullptr)
{}

StackFrame::StackFrame(domain::Callable *callee):
    mLastProgramCount(-1),
    mEvaluationStack(new EvaluationStack()),
    mCallee(callee)
{ }

StackFrame::~StackFrame()
{
    delete mEvaluationStack;
}

EvaluationStack *StackFrame::evalutaionStack()
{
    return this->mEvaluationStack;
}

const EvaluationStack *StackFrame::evalutaionStack() const
{
    return this->mEvaluationStack;
}

void StackFrame::setCallee(domain::Callable *callee)
{
    mCallee = callee;
}

domain::Callable *StackFrame::callee()
{
    return mCallee;
}

int StackFrame::lastProgramCount() const
{
    return mLastProgramCount;
}

void StackFrame::setLastProgramCount(int pc)
{
    mLastProgramCount = pc;
}

bool StackFrame::declaraVariable(int id, domain::Value *val) {
    return mVariableMap.insert(std::pair<int, domain::Value*>(id, val)).second;
}

domain::Value *StackFrame::removeVariable(int id) {
    auto find = mVariableMap.find(id);
    auto end  = mVariableMap.end();
    if(find  != end ) {
        domain::Value* removeValue = (*find).second;
        mVariableMap.erase(find);

        return removeValue;
    } else {
        return nullptr;
    }
}

domain::Value *StackFrame::variable(int id) {
    auto find = mVariableMap.find(id);
    auto end  = mVariableMap.end();
    return find != end ? (*find).second : nullptr;
}

//----------------------------------------------------------------------------

VariableCreator::VariableCreator(gc::GarbageCollector *gc):
    mGC(gc),
    mArrayCreator(new domain::ArrayCreator(gc)),
    mObjectCreator(new domain::ObjectCreator(gc)),
    //        callableCreator(new domain::CallableCreator(gc)),
    mValueCreator(new domain::ValueCreator(gc))
{ }

VariableCreator::~VariableCreator()
{
    delete mArrayCreator;
    delete mObjectCreator;
    //        delete callableCreator;
    delete mValueCreator;
}

domain::Value *VariableCreator::createValue()
{
    return mValueCreator->create();
}

domain::Value *VariableCreator::createValue(bool val)
{
    return mValueCreator->create(val);
}

domain::Value *VariableCreator::createValue(int val)
{
    return mValueCreator->create(val);
}

domain::Value *VariableCreator::createValue(double val)
{
    return mValueCreator->create(val);
}

domain::Value *VariableCreator::createValue(const std::string &val)
{
    return mValueCreator->create(val);
}

domain::Value *VariableCreator::createValue(domain::Array *val)
{
    return mValueCreator->create(val);
}

domain::Value *VariableCreator::createValue(domain::Object *val)
{
    return mValueCreator->create(val);
}

domain::Value *VariableCreator::createValue(domain::Callable *val)
{
    return mValueCreator->create(val);
}

domain::Array *VariableCreator::createArray()
{
    return mArrayCreator->create();
}

domain::Object *VariableCreator::createObject()
{
    return mObjectCreator->create();
}

gc::GarbageCollector *VariableCreator::gc()
{
    return mGC;
}

const gc::GarbageCollector *VariableCreator::gc() const
{
    return mGC;
}

void VariableCreator::destroy(gc::GCObject *o)
{
    mGC->removeObject(o);
    mGC->removeRoot(o);
//    mGC->unpin(o);
    if(!mGC->isPinObject(o)) {
        delete o;
    }
}

//----------------------------------------------------------------------------------

VirtualMachineContext::VirtualMachineContext(gc::GarbageCollector *gc):
    mVariableCreator(new VariableCreator(gc)),
    mStackFrameStack(new std::vector<StackFrame*>())
{
    mStackFrameStack->push_back(new StackFrame());          // rootStackFrame
}

VirtualMachineContext::~VirtualMachineContext()
{
    delete mVariableCreator;

    auto iter = mStackFrameStack->begin();
    auto end  = mStackFrameStack->end();
    while(iter != end) {

        delete (*iter);

        iter ++;
    }

    delete mStackFrameStack;
}

StackFrame *VirtualMachineContext::rootStackFrame()
{
    return mStackFrameStack->front();
}

const StackFrame *VirtualMachineContext::rootStackFrame() const
{
    return mStackFrameStack->front();
}

StackFrame *VirtualMachineContext::currentStackFrame()
{
    return mStackFrameStack->back();
}

const StackFrame *VirtualMachineContext::currentStackFrame() const
{
    return mStackFrameStack->back();
}

void VirtualMachineContext::push(StackFrame *stackFrame)
{
    std::cout << "VirtualMachineContext::push: [" << stackFrame << "]" << std::endl;
    mStackFrameStack->push_back(stackFrame);
}

StackFrame *VirtualMachineContext::pop() {
    if(mStackFrameStack->size() > 1) {
        StackFrame* popStackFrame = mStackFrameStack->back();
        mStackFrameStack->pop_back();

        std::cout << "VirtualMachineContext::push: [" << popStackFrame << "]" << std::endl;

        return popStackFrame;
    } else {
        return nullptr;
    }
}

size_t VirtualMachineContext::depth() const
{
    return mStackFrameStack->size();
}

VariableCreator *VirtualMachineContext::variableCreator()
{
    return mVariableCreator;
}

bool VirtualMachineContext::declaraVariable(StackFrame *stackFrame, int id, domain::Value *val) {
    assert(val != nullptr);
    mVariableCreator->gc()->addRoot(val);
    return stackFrame->declaraVariable(id, val);
}

domain::Value *VirtualMachineContext::removeVariable(StackFrame *stackFrame, int id) {
    domain::Value* removeValue = stackFrame->removeVariable(id);
    mVariableCreator->gc()->removeRoot(removeValue);
    return removeValue;
}

domain::Value *VirtualMachineContext::variable(StackFrame *stackFrame, int id) {
    return stackFrame->variable(id);
}







gc::GarbageCollector *VirtualMachineContext::gc()
{
    return mVariableCreator->gc();
}

const gc::GarbageCollector *VirtualMachineContext::gc() const
{
    return mVariableCreator->gc();
}

std::vector<StackFrame *> *VirtualMachineContext::stackFrameStack() {
    return mStackFrameStack;
}

const std::vector<StackFrame *> *VirtualMachineContext::stackFrameStack() const
{
    return mStackFrameStack;
}

//--------------------------------------------------------------------------




} // namespace context
} // namespace qyvlik

