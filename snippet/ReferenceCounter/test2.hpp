#ifndef TEST2_HPP
#define TEST2_HPP

#include "src/value.hpp"
#include <algorithm>

using namespace qyvlik;

void test2()
{
    ObjectCreator<IntegerValue> int_creator;
    ObjectCreator<FunctionValue> fun_creator;
    Reference<FunctionValue> fun_log = fun_creator.create([](const Arguments &args, Reference<Value> result, Reference<Value> error){
        (void)result;
        (void)error;
        auto iter = args.begin();
        auto end  = args.end();
        while(iter != end) {
            std::cout << (*iter)->toString() << " ";
            iter++;
        }
        std::cout << std::endl;
    });
    Reference<IntegerValue> a = int_creator.create(10);

    std::cout << a->toInteger() << std::endl;
    std::cout << a->toDouble() << std::endl;
    std::cout << a->toString() << std::endl;
    std::cout << "(a->toArray() == NullValue::nullArray()): "
              << (a->toObject() == NullValue::nullArray()) << std::endl;
    std::cout << "(a->toArray() == NullValue::nullObject()): "
              << (a->toObject() == NullValue::nullObject()) << std::endl;


    Reference<Value> a_int (a.pointer());
    Arguments arguments;
    arguments.push_back(a_int);
    fun_log->execute(arguments, NullValue::null(), NullValue::null());

    int_creator.destroy(a);
    fun_creator.destroy(fun_log);
}

#endif // TEST2_HPP
