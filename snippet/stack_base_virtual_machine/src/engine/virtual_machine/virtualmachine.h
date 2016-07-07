#ifndef QYVLIK_VIRTUAL_MACHINE_VIRTUALMACHINE_H
#define QYVLIK_VIRTUAL_MACHINE_VIRTUALMACHINE_H

#include "../../declara_forword.hpp"

namespace qyvlik {
namespace virtual_machine {

class OperatioinCode;
class VirtualMachine;

class OperatioinCode
{
public:
    OperatioinCode(int op, int id):
        mOperation(op),
        mID(id),
        mCommment("")
    {}

    OperatioinCode(int op, int id, const std::string& comment):
        mOperation(op),
        mID(id),
        mCommment(comment)
    {}

    int mOperation;
    int mID;
    std::string mCommment;

public:
    // <lable, pc>
    static std::map<int, int> LabelToProgramCount(const std::vector<OperatioinCode>& codes, const int& lableEnumValue)
    {
        size_t iter = 0;
        size_t size = codes.size();
        std::map<int, int> lines;
        while(iter < size) {
            auto& item = codes.at(iter);

            if(item.mOperation == lableEnumValue) {
                // lines.push_back(iter);
                lines[item.mID] = iter;
            }

            iter++;
        }
        return lines;
    }

    // <method_id, start_pc, end_pc>

    static std::map< int, std::tuple<int, int> > EndMethodToProgramCount(const std::vector<OperatioinCode>& codes,
                                                 const int& methodEnumValue,
                                                 const int& endMethodEnumValue)
    {
        size_t iter = 0;
        size_t size = codes.size();
        std::map<int, std::tuple<int, int> > end_method_of_pc;
        std::stack< std::tuple<int, int> > methods;
        while(iter < size) {
            auto& item = codes.at(iter);

            if(item.mOperation == methodEnumValue) {
                methods.push(std::tuple<int, int>(item.mID, iter));       // method_id, start_pc

            } else if(item.mOperation == endMethodEnumValue) {
                if(item.mID == std::get<0>(methods.top())) {
                    end_method_of_pc[item.mID] = std::tuple<int, int>(std::get<1>(methods.top()), iter);            // start_pc, end_pc
                    methods.pop();
                }
            }

            iter++;
        }
        return end_method_of_pc;
    }
};

class VirtualMachine
{
public:
    enum Operation {
        Add,
        Sub,
        Mul,
        Div,
        Mod,

        Assign,
        AddAssign,
        SubAssign,
        MulAssign,
        DivAssign,
        ModAssign,

        Equals,
        NotEquals,
        LessThanEquals,

        Push,
        Pop,
        PushTop,
        SwapTop,

        JumpIfNotZero,

        JumpIfZero,

        Jump,

        TypeName,                           // bool->0, int->1, double->2, string->3
        ClearTypeName,
        Declara,
        UnDeclara,                          // 变量移除作用域


        BeforeCall,                         // BeforeCall methodNameId
        Param,
        Call,                               // Call methodNameId

        Method,
        ParamDeclara,
        Return,
        EndMethod,

        Print,

        Label,

        ClearEvaluationStack,
    };

    explicit VirtualMachine(gc::GarbageCollector* gc);

    ~VirtualMachine();

    context::EvaluationStack* evaluationStack();

    const context::EvaluationStack* evaluationStack() const;

    std::vector<context::StackFrame*>* stackFrameStack();

    const std::vector<context::StackFrame*>* stackFrameStack() const;

    size_t stackFrameStackDepth() const;

    gc::GarbageCollector* gc();

    const gc::GarbageCollector* gc() const;

    context::VariableCreator* variableCreator();

    void init(const std::map<int, domain::Value*>& globalVariable);

    void clear();

    domain::Value* execute(const std::vector<OperatioinCode>& codes) throw(std::string);

protected:

    void push(domain::Value* val) throw(std::string);

    domain::Value* pop() throw(std::string);

    void push_top() throw(std::string);

    domain::Value* swap_top(domain::Value* val) throw(std::string);

    int jump_if_not_zero(int label) throw(std::string);

    int jump_if_zero(int label) throw(std::string);

    void print_top();

    void clear_evaluation_stack() throw(std::string);

    void add() throw(std::string);

    void sub() throw(std::string);

    void mul() throw(std::string);

    void div() throw(std::string);

    void mod() throw(std::string);

    void equals() throw(std::string);

    void not_equals() throw(std::string);

    void less_than_or_equals() throw(std::string);

    // v0
    // v1
    // =
    // v1 = v0
    void assign() throw(std::string);

    void call_native_method(int id, std::vector<domain::Arguments>* arguments_stack) throw(std::string);

protected:
    domain::Value* createValue(int type_id) throw(std::string);

private:
    context::VirtualMachineContext* mVirtualMachineContext;
    size_t mProgramCount;
    int mTypeID;
};

} // namespace virtual_machine
} // namespace qyvlik

#endif // QYVLIK_VIRTUAL_MACHINE_VIRTUALMACHINE_H
