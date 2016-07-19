#ifndef DECLARA_FORWORD_HPP
#define DECLARA_FORWORD_HPP

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <tuple>
#include <functional>
#include <set>

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

namespace gc {
class GCObject;
class GarbageCollector;
}

namespace trigger {

class OnHappenedHandle;
class OnHandle;

class OnHappened;
class OnDestroy;

class ConnectionManager;

class ValueHandle;

}

}

#endif // DECLARA_FORWORD_HPP

