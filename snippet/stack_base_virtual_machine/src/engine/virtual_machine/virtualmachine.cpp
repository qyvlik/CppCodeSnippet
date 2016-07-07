#include "virtualmachine.h"

#include "../../domain/value.h"
#include "../../domain/array.h"
#include "../../domain/callable.h"
#include "../../domain/object.h"

#include "../context/virtualmachinecontext.h"

namespace qyvlik {
namespace virtual_machine {

class ScriptCallable : public domain::Callable
{
public:
    explicit ScriptCallable(const std::string& name):
        Callable()
    {
        this->setName(name);
    }

protected:
    void exec(const domain::Arguments &, domain::Value *, domain::Value *) override
    { }
};

VirtualMachine::VirtualMachine(gc::GarbageCollector *gc):
    mVirtualMachineContext(new context::VirtualMachineContext(gc)),
    mProgramCount(0),
    mTypeID(-1)
{ }

VirtualMachine::~VirtualMachine()
{
    delete mVirtualMachineContext;
}

context::EvaluationStack *VirtualMachine::evaluationStack()
{
    return mVirtualMachineContext->currentStackFrame()->evalutaionStack();
}

const context::EvaluationStack *VirtualMachine::evaluationStack() const
{
    return mVirtualMachineContext->currentStackFrame()->evalutaionStack();
}

std::vector<context::StackFrame *> *VirtualMachine::stackFrameStack()
{
    return mVirtualMachineContext->stackFrameStack();
}

const std::vector<context::StackFrame *> *VirtualMachine::stackFrameStack() const
{
    return mVirtualMachineContext->stackFrameStack();
}

size_t VirtualMachine::stackFrameStackDepth() const
{
    return mVirtualMachineContext->depth();
}

gc::GarbageCollector *VirtualMachine::gc()
{
    return mVirtualMachineContext->gc();
}

const gc::GarbageCollector *VirtualMachine::gc() const
{
    return mVirtualMachineContext->gc();
}

context::VariableCreator *VirtualMachine::variableCreator()
{
    return mVirtualMachineContext->variableCreator();
}

void VirtualMachine::init(const std::map<int, domain::Value *> &globalVariable)
{
    //        mVirtualMachineContext
    auto iter = globalVariable.begin();
    auto end  = globalVariable.end();
    while(iter != end) {

        mVirtualMachineContext->declaraVariable(mVirtualMachineContext->rootStackFrame(), (*iter).first, (*iter).second);

        //        mVirtualMachineContext->declaraVariable((*iter).first, (*iter).second);

        iter++;
    }
}

void VirtualMachine::clear()
{}

domain::Value *VirtualMachine::execute(const std::vector<OperatioinCode> &codes) throw(std::string)
{
    mProgramCount = 0;
    mTypeID = -1;
    size_t codesSize = codes.size();
    int jump_lable = -1;

    std::map<int, int> label_code_to_pc = OperatioinCode::LabelToProgramCount(codes, Label);

    std::map< int, std::tuple<int, int> > method_id_to_pc = OperatioinCode::EndMethodToProgramCount(codes, Method, EndMethod);


    std::vector<domain::Arguments> arguments_stack;


    while(mProgramCount < codesSize) {
        auto& instruction = codes.at(mProgramCount);

        int operation = instruction.mOperation;
        int id = instruction.mID;

        switch(operation)
        {
        case Add           :
            add();
            break;

        case Sub           :
            sub();
            break;

        case Mul           :
            mul();
            break;

        case Div           :
            div();
            break;

        case Mod           :
            mod();
            break;

        case Assign:
            assign();
            break;

        case Equals         :
            equals();
            break;

        case NotEquals      :
            not_equals();
            break;

        case LessThanEquals:
            less_than_or_equals();
            break;

        case Push          : {
            domain::Value* id_value = mVirtualMachineContext->variable(mVirtualMachineContext->currentStackFrame(), id);
            if(id_value == nullptr) {
                id_value = mVirtualMachineContext->rootStackFrame()->variable(id);
            }
            push(id_value);
        }
            break;

        case PushTop       :
            push_top();
            break;

        case Pop           :
            pop();
            break;

//        case SwapTop       :
//            swap_top(mVirtualMachineContext->variable(mVirtualMachineContext->currentStackFrame(), id));
//            break;

        case TypeName:
            mTypeID = id;
            break;

        case ClearTypeName:
            mTypeID = -1;
            break;

        case Declara: {
            domain::Value* popValue = nullptr;
            if(!this->evaluationStack()->empty()) {
                popValue = this->evaluationStack()->pop();
            }
            domain::Value* newValue = createValue(mTypeID);
            newValue->assign(popValue);
            //            mVirtualMachineContext->declaraVariable(id, newValue);
            mVirtualMachineContext->declaraVariable(mVirtualMachineContext->currentStackFrame(), id, newValue);

        }
            break;
        case UnDeclara:
            mVirtualMachineContext->variableCreator()->destroy(mVirtualMachineContext->removeVariable(mVirtualMachineContext->currentStackFrame(), id));
            break;

        case JumpIfNotZero :
            jump_lable = jump_if_not_zero(label_code_to_pc[id]);
            if(jump_lable != -1) {
                mProgramCount = jump_lable;
                continue;
            }
            break;

        case JumpIfZero    :
            jump_lable = jump_if_zero(label_code_to_pc[id]);
            if(jump_lable != -1) {
                mProgramCount = jump_lable;
                continue;
            }
            break;

        case Jump          :
            mProgramCount = label_code_to_pc[id];
            continue;
            // break;

            /*
        Method,
        ParamDeclara,
        Return,
        EndMethod,*/

        case Method: {
            int method_end_pc  = std::get<1>( method_id_to_pc[id]);

            domain::Callable* currentCallable = this->mVirtualMachineContext->currentStackFrame()->callee();

            if(currentCallable != nullptr) {
                if(currentCallable->name() != std::to_string(id)) {
                    mProgramCount = method_end_pc;
                }
            } else {
                mProgramCount = method_end_pc;
            }
        }
            // TODO
            break;

        case EndMethod:
            break;

        case Return: {
            // id = -1
            arguments_stack.pop_back();
            domain::Value* method_result = nullptr;
            if(id == -1) {
                method_result = this->evaluationStack()->pop();
            } else {
                method_result = this->mVirtualMachineContext->variable(mVirtualMachineContext->currentStackFrame(), id);
                if(method_result == nullptr) {
                    method_result = mVirtualMachineContext->rootStackFrame()->variable(id);
                }
            }
            context::StackFrame* stack_frame = this->mVirtualMachineContext->pop();
            int last_program_count = stack_frame->lastProgramCount();
            domain::Callable *script_callable = stack_frame->callee();

            delete script_callable;
            delete stack_frame;

            push(method_result);

            // TODO
            mProgramCount = last_program_count;
        }
            break;

        case ParamDeclara: {
            // 函数传递参数
            // check param type
            // 在递归调用的时候
            domain::Arguments& arguments = arguments_stack.back();

            domain::Value* param = arguments.front();

            if(param->type() != mTypeID) {
                std::cerr << "\tparam type error! " << std::endl;
            }

            domain::Value* newValue = createValue(mTypeID);
            newValue->assign(param);

            // mVirtualMachineContext->declaraVariable(id, newValue);
            mVirtualMachineContext->declaraVariable(mVirtualMachineContext->currentStackFrame(), id, newValue);

            arguments.erase(arguments.begin());
        }
            break;

            // BeforeCall methodId
        case BeforeCall    :
            // arguments.clear();
            arguments_stack.push_back(domain::Arguments());
            break;

        case Param         :
            if(id != -1) {
                arguments_stack.back().push_back(mVirtualMachineContext->variable(mVirtualMachineContext->currentStackFrame(), id));
            } else {
                // Param -1
                arguments_stack.back().push_back(pop());
            }
            break;

        case Call          : {
            // 查找c++函数
            // TODO 设计脚本函数

            // 没有在脚本定义的函数列表中
            if(method_id_to_pc.find(id) == method_id_to_pc.end()) {
                call_native_method(id, &arguments_stack);
            } else {
                int method_start_pc = std::get<0>( method_id_to_pc[id]);
                // int method_end_pc   = std::get<1>( method_id_to_pc[id]);

                domain::Callable *script_callable = new ScriptCallable(std::to_string(id));
                context::StackFrame* stack_frame = new context::StackFrame(script_callable);

                stack_frame->setLastProgramCount(mProgramCount);

                mVirtualMachineContext->push(stack_frame);

                mProgramCount = method_start_pc;
            }
        }
            break;

        case Print         :
            print_top();
            break;

        case ClearEvaluationStack:
            clear_evaluation_stack();
            break;

        case Label         :
        default            :
            break;
        }

        mProgramCount++;
    }

    return this->evaluationStack()->top();
}

void VirtualMachine::push(domain::Value *val) throw(std::string)
{
    if(val == nullptr) {
        std::cerr << "Virtual Machine Operation Push push nullptr!" << std::endl;
    }
    this->evaluationStack()->push(val);
}

domain::Value *VirtualMachine::pop() throw(std::string)
{
    if(!this->evaluationStack()->empty()) {
        return this->evaluationStack()->pop();
    } else {
        return nullptr;
    }
}

void VirtualMachine::push_top() throw(std::string)
{
    if(!this->evaluationStack()->empty()) {
        return this->evaluationStack()->push(this->evaluationStack()->top());
    }
}

domain::Value *VirtualMachine::swap_top(domain::Value *val) throw(std::string)
{
    domain::Value* old_top = this->evaluationStack()->pop();
    this->evaluationStack()->push(val);
    return old_top;
}

int VirtualMachine::jump_if_not_zero(int label) throw(std::string)
{
    if(! this->evaluationStack()->top()->toBoolean() ) {
        this->pop();
        return label;
    } else {
        this->pop();
        return -1;
    }
}

int VirtualMachine::jump_if_zero(int label) throw(std::string)
{
    if( !this->evaluationStack()->top()->toBoolean() ) {
        this->pop();
        return label;
    } else {
        this->pop();
        return -1;
    }
}

void VirtualMachine::print_top()
{
    std::cout << "\tTop: " << this->evaluationStack()->top()->toString() << std::endl;
}

void VirtualMachine::clear_evaluation_stack() throw(std::string)
{
    this->evaluationStack()->clear();
}

void VirtualMachine::add() throw(std::string)
{
    domain::Value* v0 = this->evaluationStack()->pop();
    domain::Value* v1 = this->evaluationStack()->pop();
    int max_type = std::max(v0->type(), v1->type());
    context::VariableCreator* variableCreator = this->mVirtualMachineContext->variableCreator();

    switch(max_type)
    {
    case domain::Value::Integer:
        push(variableCreator->createValue(v0->toInteger()+v1->toInteger()));
        break;
    case domain::Value::Double:
        push(variableCreator->createValue(v0->toDouble()+v1->toDouble()));
        break;
    case domain::Value::String:
        push(variableCreator->createValue(v0->toString()+v1->toString()));
        break;
    case domain::Value::Boolean:
    default:
        throw std::string("TypeError!") + std::to_string(mProgramCount);
    }
}

void VirtualMachine::sub() throw(std::string)
{
    domain::Value* v0 = this->evaluationStack()->pop();
    domain::Value* v1 = this->evaluationStack()->pop();
    int max_type = std::max(v0->type(), v1->type());
    context::VariableCreator* variableCreator = this->mVirtualMachineContext->variableCreator();

    switch(max_type)
    {
    case domain::Value::Integer:
        push(variableCreator->createValue(v1->toInteger()-v0->toInteger()));
        break;
    case domain::Value::Double:
        push(variableCreator->createValue(v1->toDouble()-v0->toDouble()));
        break;
    case domain::Value::String:
    case domain::Value::Boolean:
    default:
        throw std::string("TypeError!") + std::to_string(mProgramCount);
    }
}

void VirtualMachine::mul() throw(std::string)
{
    domain::Value* v0 = this->evaluationStack()->pop();
    domain::Value* v1 = this->evaluationStack()->pop();
    int max_type = std::max(v0->type(), v1->type());
    context::VariableCreator* variableCreator = this->mVirtualMachineContext->variableCreator();

    switch(max_type)
    {

    case domain::Value::Integer:
        push(variableCreator->createValue(v0->toInteger()*v1->toInteger()));
        break;
    case domain::Value::Double:
        push(variableCreator->createValue(v0->toDouble()*v1->toDouble()));
        break;
    case domain::Value::String:
    case domain::Value::Boolean:
    default:
        throw std::string("TypeError!") + std::to_string(mProgramCount);
    }
}

void VirtualMachine::div() throw(std::string)
{
    domain::Value* v0 = this->evaluationStack()->pop();
    domain::Value* v1 = this->evaluationStack()->pop();
    int max_type = std::max(v0->type(), v1->type());
    context::VariableCreator* variableCreator = this->mVirtualMachineContext->variableCreator();

    switch(max_type)
    {
    case domain::Value::Integer:
        if(v1->toInteger() == 0) {
            throw std::string("Value is Zero: ") + std::to_string(mProgramCount);
        }
        push(variableCreator->createValue(v0->toInteger() / v1->toInteger()));
        break;
    case domain::Value::Double:
        if(v1->toDouble() == 0) {
            throw std::string("Value is Zero: ") + std::to_string(mProgramCount);
        }
        push(variableCreator->createValue(v0->toDouble() / v1->toDouble()));
        break;
    case domain::Value::String:
    case domain::Value::Boolean:
    default:
        throw std::string("TypeError!: ") + std::to_string(mProgramCount);
    }
}

void VirtualMachine::mod() throw(std::string)
{
    domain::Value* v0 = this->evaluationStack()->pop();
    domain::Value* v1 = this->evaluationStack()->pop();
    int max_type = std::max(v0->type(), v1->type());
    context::VariableCreator* variableCreator = this->mVirtualMachineContext->variableCreator();

    switch(max_type)
    {
    case domain::Value::Integer:
        if(v1->toInteger() == 0) {
            throw std::string("Value is Zero: ") + std::to_string(mProgramCount);
        }
        push(variableCreator->createValue(v0->toInteger() % v1->toInteger()));
        break;
    case domain::Value::Double:
    case domain::Value::String:
    case domain::Value::Boolean:
    default:
        throw std::string("TypeError!: ") + std::to_string(mProgramCount);
    }
}

void VirtualMachine::equals() throw(std::string)
{
    domain::Value* v0 = this->evaluationStack()->pop();
    domain::Value* v1 = this->evaluationStack()->pop();
    context::VariableCreator* variableCreator = this->mVirtualMachineContext->variableCreator();

    push(variableCreator->createValue(v0->equals(v1)));
}

void VirtualMachine::not_equals() throw(std::string)
{
    domain::Value* v0 = this->evaluationStack()->pop();
    domain::Value* v1 = this->evaluationStack()->pop();
    context::VariableCreator* variableCreator = this->mVirtualMachineContext->variableCreator();

    push(variableCreator->createValue(!v0->equals(v1)));
}

// <=
void VirtualMachine::less_than_or_equals() throw(std::string)
{
    domain::Value* v0 = this->evaluationStack()->pop();
    domain::Value* v1 = this->evaluationStack()->pop();
    int max_type = std::max(v0->type(), v1->type());

    context::VariableCreator* variableCreator = this->mVirtualMachineContext->variableCreator();

    switch(max_type)
    {
    case domain::Value::Integer:
        push(variableCreator->createValue(v1->toInteger() <= v0->toInteger()));
        break;
    case domain::Value::Double:
        push(variableCreator->createValue(v1->toDouble() <= v0->toDouble()));

    case domain::Value::Boolean:
    case domain::Value::String:
    default:
        throw std::string("TypeError!: ") + std::to_string(mProgramCount);
    }
}

void VirtualMachine::assign() throw(std::string)
{
    domain::Value* v0 = this->evaluationStack()->pop();
    domain::Value* v1 = this->evaluationStack()->pop();

    v1->assign(v0);
    push(v1);
}

void VirtualMachine::call_native_method(int id, std::vector<domain::Arguments> *arguments_stack) throw(std::string)
{
    context::StackFrame* stack_frame = new context::StackFrame(mVirtualMachineContext->variable(mVirtualMachineContext->currentStackFrame(), id)->toCallable());

    mVirtualMachineContext->push(stack_frame);

    domain::Value* result = mVirtualMachineContext->variableCreator()->createValue();
    domain::Value* error = mVirtualMachineContext->variableCreator()->createValue();

    context::CallContext call_context(this, arguments_stack->back(), result, error);
    arguments_stack->pop_back();

    mVirtualMachineContext->currentStackFrame()->callee()->call(&call_context);

    if(error->type() == domain::Value::Undefined) {

        mVirtualMachineContext->pop();
        delete stack_frame;

        push(result);
    } else {
        // throw std::string("Error!");
        std::cerr << "mProgramCount: " << mProgramCount << std::endl;
        std::cerr << "Call Error: " << error->toString() << std::endl;
    }
}

domain::Value *VirtualMachine::createValue(int type_id) throw(std::string)
{
    switch(type_id)
    {
    case domain::Value::Boolean:
        return mVirtualMachineContext->variableCreator()->createValue((bool)false);
    case domain::Value::Integer:
        return mVirtualMachineContext->variableCreator()->createValue((int)0);
    case domain::Value::Double:
        return mVirtualMachineContext->variableCreator()->createValue((double)0);
    case domain::Value::String:
        return mVirtualMachineContext->variableCreator()->createValue(std::string(""));
    case domain::Value::Object:
        return mVirtualMachineContext->variableCreator()->createValue(mVirtualMachineContext->variableCreator()->createObject());
    case domain::Value::Array:
        return mVirtualMachineContext->variableCreator()->createValue(mVirtualMachineContext->variableCreator()->createArray());
        // TODO
    case domain::Value::Function:
    default:
        std::cerr << "TypeError! : " << type_id;
        throw std::string("TypeError! Type not exitis");
        return nullptr;
    }
}



} // namespace virtual_machine
} // namespace qyvlik

