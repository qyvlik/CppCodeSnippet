#include <iostream>

#include <thread>
#include <QObject>

#include "src/engine/virtual_machine/virtualmachine.h"
#include "src/engine/context/virtualmachinecontext.h"
#include "src/domain/array.h"
#include "src/domain/value.h"
#include "src/domain/callable.h"
#include "src/domain/object.h"


using namespace qyvlik;
using namespace qyvlik::context;
using namespace qyvlik::domain;
using namespace qyvlik::gc;
using namespace qyvlik::virtual_machine;

void test_0();
void test_1();
void test_2();
void test_3();
void test_4();

int f(int a) {
    if( a <= 1) {
        return 1;
    } else {
        return f( a - 1) + f(a-2);
    }
}

class gc_object
{
public:
    virtual ~gc_object(){}
    bool marked:1;
    bool busy:1;
    bool thread:1;
    bool resuable:1;
    bool eden:1;
    bool survivor:1;
    bool tenured:1;
    bool a3:1;
};

int main()
{
    //test_3();
//   std::cout << f(10) << std::endl;
     test_4();


    //std::cout << std::this_thread::get_id();
    //std::cout << sizeof(gc_object) <<std::endl;

    return 0;
}

/*
.global
1 print
2 track
3 20

.code
TypeName        1       ; int
Push            3       ; 20
Declara         4       ; a
Push            3       ; 20
Declara         5       ; b
Push            4       ; a
Push            5       ; b
Add             _
Print           _
UnDeclara       4       ; a
UnDeclara       5       ; b
*/
void test_0()
{
    gc::GarbageCollector gc;

    CallableCreator buildInCallee(&gc);

    VirtualMachine vm(&gc);

    std::map<int, domain::Value*> global;
    global[1] = vm.variableCreator()->createValue(buildInCallee.create("print"));
    global[2] = vm.variableCreator()->createValue(buildInCallee.create("track"));
    global[3] = vm.variableCreator()->createValue(20);
    vm.init(global);

    std::vector<OperatioinCode> codes;

    // int a = 20, b = 20
    // echo a+b
    codes.push_back(OperatioinCode(VirtualMachine::TypeName,    Value::Integer, "; int"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,        3,              "; 20"));
    codes.push_back(OperatioinCode(VirtualMachine::Declara,     4,              "; a"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,        3,              "; 20"));
    codes.push_back(OperatioinCode(VirtualMachine::Declara,     5,              "; b"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,        4,              "; a"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,        5,              "; b"));
    codes.push_back(OperatioinCode(VirtualMachine::Add,         0));
    codes.push_back(OperatioinCode(VirtualMachine::Print,       0));
    //    codes.push_back(OperatioinCode(VirtualMachine::UnDeclara,   4,              "; a"));
    //    codes.push_back(OperatioinCode(VirtualMachine::UnDeclara,   5,              "; b"));

    vm.execute(codes);

    gc.collect(true);
}


/*
.global
1 print
2 track
3 20

.code
TypeName        1       ; int
Push            3       ; 20
Declara         4       ; a
Push            3       ; 20
Declara         5       ; b
BeforeCall      1       ; print
BeforeCall      1       ; print
Param           4       ; a
Param           5       ; b
Call            1       ;
Param          -1       ; b
Call            1       ;

*/
void test_1()
{
    gc::GarbageCollector gc;

    CallableCreator buildInCallee(&gc);

    VirtualMachine vm(&gc);

    std::map<int, domain::Value*> global;
    global[1] = vm.variableCreator()->createValue(buildInCallee.create("print"));
    global[2] = vm.variableCreator()->createValue(buildInCallee.create("track"));
    global[3] = vm.variableCreator()->createValue(20);
    vm.init(global);


    std::vector<OperatioinCode> codes;

    // int a = 20, b = 20;
    // print( print(a, b) );
    codes.push_back(OperatioinCode(VirtualMachine::TypeName,    Value::Integer, "; int"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,        3,              "; 20"));
    codes.push_back(OperatioinCode(VirtualMachine::Declara,     4,              "; a"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,        3,              "; 20"));
    codes.push_back(OperatioinCode(VirtualMachine::Declara,     5,              "; b"));
    codes.push_back(OperatioinCode(VirtualMachine::BeforeCall,  1,              "; print"));
    codes.push_back(OperatioinCode(VirtualMachine::BeforeCall,  1,              "; print"));
    codes.push_back(OperatioinCode(VirtualMachine::Param,       4,              "; a"));
    codes.push_back(OperatioinCode(VirtualMachine::Param,       5,              "; b"));
    codes.push_back(OperatioinCode(VirtualMachine::Call,        1,              "; print(a, b)"));
    codes.push_back(OperatioinCode(VirtualMachine::Param,      -1,              "; last time result"));
    codes.push_back(OperatioinCode(VirtualMachine::Call,        1,              "; print(2)"));


    vm.execute(codes);
    gc.collect(true);
}

void test_2()
{
    std::vector<OperatioinCode> codes;

    // int a = 20, b = 20
    // echo a+b
    codes.push_back(OperatioinCode(VirtualMachine::Method,  1));
    codes.push_back(OperatioinCode(VirtualMachine::Method,  2));
    codes.push_back(OperatioinCode(VirtualMachine::EndMethod,  2));
    codes.push_back(OperatioinCode(VirtualMachine::EndMethod,  1));

    auto m_pc = OperatioinCode::EndMethodToProgramCount(codes, VirtualMachine::Method, VirtualMachine::EndMethod);
    auto iter = m_pc.begin();
    auto end  = m_pc.end();
    while(iter != end) {

        std::cout << "method id: " << iter->first
                  << "method start pc: " << std::get<0>(iter->second)
                  << "method end pc:" << std::get<1>(iter->second)
                  << std::endl;

        iter++;
    }
}


/*
.global
1 print
2 track
3 20

.code
Method              4    ; add
TypeName            1    ; int
ParamDeclara        5    ; a
TypeName            1    ; int
ParamDeclara        6    ; int
Push                5    ; a
Push                6    ; b
Add                 _    ;
Return              -1    ;
EndMethod           4    ; end of add

TypeName            1       ; int
Push                3       ; 20
Declara             7       ; a
Push                3       ; 20
Declara             8       ; b

TypeName            1       ; int
BeforeCall          4       ; add
Param               7       ; a
Param               8       ; b
Call                4       ; add
Declara             9       ; c

BeforeCall          1       ; print
Param               9       ; c
Call                1       ; print
*/

void test_3()
{
    gc::GarbageCollector gc;

    CallableCreator buildInCallee(&gc);

    VirtualMachine vm(&gc);

    std::map<int, domain::Value*> global;
    global[1] = vm.variableCreator()->createValue(buildInCallee.create("print"));
    global[2] = vm.variableCreator()->createValue(buildInCallee.create("track"));
    global[3] = vm.variableCreator()->createValue(20);
    vm.init(global);

    std::vector<OperatioinCode> codes;

    codes.push_back(OperatioinCode(VirtualMachine::Method,              4,              "; add"));
    codes.push_back(OperatioinCode(VirtualMachine::TypeName,            Value::Integer, "; int"));
    codes.push_back(OperatioinCode(VirtualMachine::ParamDeclara,        5,              "; a"));
    codes.push_back(OperatioinCode(VirtualMachine::TypeName,            Value::Integer, "; int"));
    codes.push_back(OperatioinCode(VirtualMachine::ParamDeclara,        6,              "; b"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,                5,              "; a"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,                6,              "; b"));
    codes.push_back(OperatioinCode(VirtualMachine::Add,                 0,              "; +"));
    codes.push_back(OperatioinCode(VirtualMachine::Return,             -1));
    codes.push_back(OperatioinCode(VirtualMachine::EndMethod,           4,              "; add"));

    codes.push_back(OperatioinCode(VirtualMachine::TypeName,            Value::Integer, "; int"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,                3,              "; 20"));
    codes.push_back(OperatioinCode(VirtualMachine::Declara,             7,              "; a"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,                3,              "; 20"));
    codes.push_back(OperatioinCode(VirtualMachine::Declara,             8,              "; b"));
    codes.push_back(OperatioinCode(VirtualMachine::TypeName,            Value::Integer, "; int"));
    codes.push_back(OperatioinCode(VirtualMachine::BeforeCall,          4,              "; add"));
    codes.push_back(OperatioinCode(VirtualMachine::Param,               7,              "; a"));
    codes.push_back(OperatioinCode(VirtualMachine::Param,               8,              "; b"));
    codes.push_back(OperatioinCode(VirtualMachine::Call,                4,              "; add(a, b)"));
    codes.push_back(OperatioinCode(VirtualMachine::Declara,             9,              "; c"));

    codes.push_back(OperatioinCode(VirtualMachine::BeforeCall,          1,              "; print"));
    codes.push_back(OperatioinCode(VirtualMachine::Param,               9,              "; c"));
    codes.push_back(OperatioinCode(VirtualMachine::Call,                1,              "; print(c)"));

    vm.execute(codes);

    gc.collect(true);
}

//int f(int a) {
//    if( a - 1 == 0) {
//        return 1;
//    } else {
//        return f( a - 1) + f(a-2);
//    }
//}

/*
.global
1 print ; method
2 1     ; number
3 2     ; number

Method              4       ; f
TypeName            1       ; int
ParamDeclara        5       ; a

Push                5       ;
Push                2       ; 1
LessThanEquals      _       ; [5] <= [2]
IfZeroJump          1       ;
Label               0       ;
Return              2       ;
Label               1       ;
BeforeCall          4       ;
Push                5       ; a
Push                2       ; 1
Sub                 _       ; a - 1
Param               -1      ; a - 1
Call                4       ; f

BeforeCall          4       ;
Push                5       ; a
Push                3       ; 2
Sub                 _       ; a - 2
Param               -1      ; a - 2
Call                4       ; f

Add                 _       ;

Return             -1       ;
EndMethod           4       ;
*/
void test_4()
{
    gc::GarbageCollector gc;

    CallableCreator buildInCallee(&gc);

    VirtualMachine vm(&gc);

    std::map<int, domain::Value*> global;
    global[1] = vm.variableCreator()->createValue(buildInCallee.create("print"));
    global[2] = vm.variableCreator()->createValue(1);
    global[3] = vm.variableCreator()->createValue(2);

    global[12] = vm.variableCreator()->createValue(10);


    vm.init(global);

    // parser code

    std::vector<OperatioinCode> codes;

    codes.push_back(OperatioinCode(VirtualMachine::Method,              4,              "; f"));
    codes.push_back(OperatioinCode(VirtualMachine::TypeName,            Value::Integer, "; int"));
    codes.push_back(OperatioinCode(VirtualMachine::ParamDeclara,        5,              "; a"));

    codes.push_back(OperatioinCode(VirtualMachine::Push,                5,              "; a"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,                2,              "; 1"));
    codes.push_back(OperatioinCode(VirtualMachine::LessThanEquals,      0,              "; <="));

    codes.push_back(OperatioinCode(VirtualMachine::JumpIfNotZero,       1,              "; jump"));
    codes.push_back(OperatioinCode(VirtualMachine::Label,               0,              "; lable 0"));
    codes.push_back(OperatioinCode(VirtualMachine::Return,              2,              "; return number 1"));

    codes.push_back(OperatioinCode(VirtualMachine::Label,               1,              "; lable 1"));
    codes.push_back(OperatioinCode(VirtualMachine::BeforeCall,          4,              "; f"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,                5,              "; a"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,                2,              "; 1"));
    codes.push_back(OperatioinCode(VirtualMachine::Sub,                 0,              "; -"));
    codes.push_back(OperatioinCode(VirtualMachine::Param,              -1,              "; a-1"));
    codes.push_back(OperatioinCode(VirtualMachine::Call,                4,              "; f(a-1)"));

    codes.push_back(OperatioinCode(VirtualMachine::BeforeCall,          4,              "; f"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,                5,              "; a"));
    codes.push_back(OperatioinCode(VirtualMachine::Push,                3,              "; 2"));
    codes.push_back(OperatioinCode(VirtualMachine::Sub,                 0,              "; -"));
    codes.push_back(OperatioinCode(VirtualMachine::Param,              -1,              "; a-2"));
    codes.push_back(OperatioinCode(VirtualMachine::Call,                4,              "; f(a-2)"));

    codes.push_back(OperatioinCode(VirtualMachine::Add,                 0,              "; f(a-1)+f(a-2)"));
    codes.push_back(OperatioinCode(VirtualMachine::Return,             -1));
    codes.push_back(OperatioinCode(VirtualMachine::EndMethod,           4,              "; f"));


    codes.push_back(OperatioinCode(VirtualMachine::BeforeCall,          4,              "; f"));
    codes.push_back(OperatioinCode(VirtualMachine::Param,              12,              "; [12]"));
    codes.push_back(OperatioinCode(VirtualMachine::Call,                4,              "; f([12])"));
    codes.push_back(OperatioinCode(VirtualMachine::Print,               0,              "; f([12])")); // [12] == 4, print 5

//    struct timeval tv_begin, tv_end;
//    gettimeofday(&tv_begin, NULL);

    vm.execute(codes);

//    gettimeofday(&tv_end, NULL);
//    std::cout << "use time: " << (tv_end.tv_sec -tv_begin.tv_sec)
//              << " seconds, " << (tv_end.tv_usec-tv_begin.tv_usec) << " milliseconds" << std::endl;



//    gc.collect(true);
}
