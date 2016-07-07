#ifndef DECLARA_FORWORD_HPP
#define DECLARA_FORWORD_HPP

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <tuple>

#include <sys/time.h>

namespace qyvlik {
namespace domain {

class Value;
class ValueCreator;

class Object;
class ObjectCreator;

class Array;
class ArrayCreator;

typedef std::vector<Value*> Arguments;
class Callable;
class CallableCreator;

}

namespace context {

class EvaluationStack;
class CallContext;
class StackFrame;
class VariableCreator;
class VirtualMachineContext;
}

namespace virtual_machine {

class OperatioinCode;
class VirtualMachine;
}

namespace gc {
class GCObject;
class GarbageCollector;
}

}

#endif // DECLARA_FORWORD_HPP

