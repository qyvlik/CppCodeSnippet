#include <iostream>

//#include "test.hpp"
//#include "test1.hpp"
#include "test2.hpp"

#include <stack>

template<typename Type>
class Stack {
public:
    Stack() = default;
    ~Stack() = default;
    void push(const Type& val) {
        mData.push(val);
    }

    Type pop() {
        Type popItem = mData.top();
        mData.pop();
        return popItem;
    }

    Type& top() {
        return mData.top();
    }

    const Type& top() const {
        return mData.top();
    }

    int depth() const {
        return mData.size();
    }
private:
    std::stack<Type> mData;
};

class VirtualMachine
{
public:
    enum Operation {
        IADD,
        ISUB,
        IMUL,
        IDIV,
        IMOD,

        PUSH,
        POP,

        IMOV,
        ISTORE,
        ILOAD
    };

    void eval(const int codes[], const int size)
    {
        int code = -1;
        int program_counter = 0;
        while(program_counter < size) {
            code = codes[program_counter];
            switch (code)
            {
            case IADD:
                mGeneralRegister->assign(mStackPointer.pop()->toInteger() + mGeneralRegister->toInteger());
                break;
            case ISUB:
                mGeneralRegister->assign(mStackPointer.pop()->toInteger() - mGeneralRegister->toInteger());
                break;
            case IMUL:
                mGeneralRegister->assign(mStackPointer.pop()->toInteger() * mGeneralRegister->toInteger());
                break;
            case IDIV:
                mGeneralRegister->assign(mStackPointer.pop()->toInteger() / mGeneralRegister->toInteger());
                break;
            case IMOD:
                mGeneralRegister->assign(mStackPointer.pop()->toInteger() % mGeneralRegister->toInteger());
                break;
            case IMOV:
                mGeneralRegister->assign(mStackPointer.pop()->toInteger());
                break;

            case PUSH:
                mStackPointer.push(mGeneralRegister);
                break;
            case POP:
                mStackPointer.pop();
                break;
            default:
                std::cerr << "Unknown Operation " << code << std::endl;
                break;
            }



            program_counter++;
        }
    }

private:
    qyvlik::Value*              mGeneralRegister;                // 通用寄存器
    qyvlik::Value*              mBasePointer;                     // 基址寄存器
    Stack<qyvlik::Value*>       mStackPointer;                    // 栈顶寄存器
    qyvlik::Value* mHead;                           // 堆
};

int main(int , char **)
{
    test2();
    return 0;
}
