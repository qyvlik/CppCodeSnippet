#include <iostream>
#include <vector>
#include <set>

struct virtual_machine
{
public:
    enum {
        Size = 10
    };
    enum Op {
        IADD = 0,
        ISTORE = 1,
        ILOAD = 2,
        IMOV = 3,
        IOFFEST = 4,
    };


    virtual_machine():
        stack(new int[Size]{0}),
        static_data(new int[Size]{0}),
        evaluate_stack(new int*[Size]{0})
    {
        ax = 0;
        sp = evaluate_stack + Size-1;
    }

    ~virtual_machine()
    {
        delete [] static_data;
        delete [] stack;
        delete [] evaluate_stack;
    }

    void iadd() {
        *ax = *(*(sp++)) + *(*(sp++));
    }

    void isub() {
        *ax = *(*(sp++)) - *(*(sp++));
    }

    void istore() {
        *(--sp) = ax;
    }

    // 在栈上偏移指定大小，然后保存地址
    void imov(int offest) {
        ax = stack + offest;
    }

    // 将运算栈栈顶元素保存，然后弹出
    void iload() {
        ax = *(sp--);
    }

    void run(const int codes[], int size) {
        int pc = 0;
        int instr = 0;
        int offest = 0;
        while(pc < size)
        {
            instr = codes[pc];
            switch (instr)
            {
            case IADD:
                this->iadd();
                break;
            case ILOAD:
                this->iload();
                break;
            case IMOV:
                this->imov(offest);
                break;
            case ISTORE:
                this->istore();
                break;
            case IOFFEST:
                offest = codes[++pc];
                break;
            default:
                std::cerr << "Unknow Instruction " << instr << std::endl;
            }
            pc++;
        }
    }


    int*    stack;
    int*    static_data;
    int**   evaluate_stack;
    int*    ax;
    int**   sp;
};

void test();

int main()
{
    virtual_machine vm;

    // init
    vm.stack[0] = 1;
    vm.stack[1] = 2;

    int codes[9] = {
        virtual_machine::IOFFEST,
        0,
        virtual_machine::IMOV,

        virtual_machine::ISTORE,

        virtual_machine::IOFFEST,
        1,
        virtual_machine::IMOV,

        virtual_machine::ISTORE,

        virtual_machine::IADD
    };

    vm.run(codes, 9);

    std::cout << "ax: [" << vm.ax << "]" << std::endl
              << "*ax: " << *(vm.ax) << std::endl;
}

