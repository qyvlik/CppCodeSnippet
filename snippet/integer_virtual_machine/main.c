// this file is used for tutorial to build the compiler step by step
// only work on 32-bit

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

int token;            // current token
char *src, *old_src;  // pointer to source code string;
int poolsize;         // default size of text/data/stack
int line;             // line number
int *text,            // text segment
    *old_text,        // for dump text segment
    *stack;           // stack
char *data;           // data segment
int *pc, *bp, *sp, ax, cycle; // virtual machine registers

// pc 程序计算器
// sp 指针寄存器
// bp 基址寄存器。用于指向栈的某些位置，在调用函数时会使用到它。
// ax 通用寄存器。用于存放指令执行后的结果


// instructions
enum {
    LEA ,IMM ,JMP ,
    CALL,JZ  ,JNZ ,
    ENT ,ADJ ,LEV ,
    LI  ,LC  ,SI  ,SC
    ,PUSH,
    OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,
    ADD ,SUB ,MUL ,DIV ,MOD ,
    OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT
};


int eval()
{
    int op, *tmp;

    while (1) {

        printf("ax: [%d]\n",   ax);
        printf("pc: [0x%p]\n", pc);
        printf("bp: [0x%p]\n", bp);
        printf("sp: [0x%p]\n", sp);

        op = *pc++; // get next operation code

        // load immediate value to ax
        if (op == IMM) {
            ax = *pc++;
        }

        // load character to ax, address in ax
        else if (op == LC)   {
            ax = *(char *)ax;
        }

        // load integer to ax, address in ax
        else if (op == LI)   {
            ax = *(int *)ax;
        }

        // save character to address, value in ax, address on stack
        else if (op == SC)   {
            ax = *(char *)*sp++ = ax;
        }

        // save integer to address, value in ax, address on stack
        else if (op == SI)   {
            *(int *)*sp++ = ax;
        }

        // push the value of ax onto the stack
        else if (op == PUSH) {
            *--sp = ax;
        }

        // jump to the address
        else if (op == JMP)  {
            pc = (int *)*pc;
        }

        // jump if ax is zero
        else if (op == JZ)   {
            pc = ax ? pc + 1 : (int *)*pc;
        }

        // jump if ax is zero
        else if (op == JNZ)  {
            pc = ax ? (int *)*pc : pc + 1;
        }

        // call subroutine
        else if (op == CALL) {
            *--sp = (int)(pc+1); pc = (int *)*pc;
        }

        //else if (op == RET)  {pc = (int *)*sp++;}
        // return from subroutine;

        // make new stack frame
        else if (op == ENT)  {
            *--sp = (int)bp;
            bp = sp;
            sp = sp - *pc++;
        }

        // add esp, <size>
        else if (op == ADJ)  {
            sp = sp + *pc++;
        }

        // restore call frame and PC
        else if (op == LEV)  {
            sp = bp;
            bp = (int *)*sp++;
            pc = (int *)*sp++;
        }

        // load address for arguments.
        else if (op == LEA)  {
            ax = (int)(bp + *pc++);
        }

        else if (op == OR)
            ax = *sp++ | ax;

        else if (op == XOR)
            ax = *sp++ ^ ax;

        else if (op == AND)
            ax = *sp++ & ax;

        else if (op == EQ)
            ax = *sp++ == ax;

        else if (op == NE)
            ax = *sp++ != ax;

        else if (op == LT)
            ax = *sp++ < ax;

        else if (op == LE)
            ax = *sp++ <= ax;

        else if (op == GT)
            ax = *sp++ >  ax;

        else if (op == GE)
            ax = *sp++ >= ax;

        else if (op == SHL)
            ax = *sp++ << ax;

        else if (op == SHR)
            ax = *sp++ >> ax;

        else if (op == ADD)
            ax = *sp++ + ax;

        else if (op == SUB)
            ax = *sp++ - ax;

        else if (op == MUL)
            ax = *sp++ * ax;

        else if (op == DIV)
            ax = *sp++ / ax;

        else if (op == MOD)
            ax = *sp++ % ax;


        else if (op == EXIT) { printf("exit(%d)", *sp); return *sp;}
        else if (op == OPEN) { ax = open((char *)sp[1], sp[0]); }
        else if (op == CLOS) { ax = close(*sp);}
        else if (op == READ) { ax = read(sp[2], (char *)sp[1], *sp); }
        else if (op == PRTF) { tmp = sp + pc[1]; ax = printf((char *)tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5], tmp[-6]); }
        else if (op == MALC) { ax = (int)malloc(*sp);}
        else if (op == MSET) { ax = (int)memset((char *)sp[2], sp[1], *sp);}
        else if (op == MCMP) { ax = memcmp((char *)sp[2], (char *)sp[1], *sp);}
        else {
            printf("unknown instruction:%d\n", op);
            return -1;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{

    int i;

    argc--;
    argv++;

    poolsize = 256 * 1024; // arbitrary size
    line = 1;

    // allocate memory for virtual machine
    if (!(text = old_text = malloc(poolsize))) {
        printf("could not malloc(%d) for text area\n", poolsize);
        return -1;
    }
    if (!(data = malloc(poolsize))) {
        printf("could not malloc(%d) for data area\n", poolsize);
        return -1;
    }
    if (!(stack = malloc(poolsize))) {
        printf("could not malloc(%d) for stack area\n", poolsize);
        return -1;
    }

    memset(text, 0, poolsize);
    memset(data, 0, poolsize);
    memset(stack, 0, poolsize);
    bp = sp = (int*)((int)stack + poolsize);
    ax = 0;

    i = 0;
    text[i++] = IMM;
    text[i++] = 10;
    text[i++] = PUSH;
    text[i++] = IMM;
    text[i++] = 20;
    text[i++] = ADD;
    text[i++] = PUSH;
    text[i++] = EXIT;

    pc = text;

    return eval();
}
