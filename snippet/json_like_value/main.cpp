#include <iostream>

#include "src/gc/gc.h"
#include "src/domain/value.h"
#include "src/domain/object.h"
#include "src/domain/array.h"
#include "src/domain/callable.h"
#include "src/gc/trigger.h"

using namespace qyvlik;
using namespace gc;
using namespace domain;

int main()
{
    GarbageCollector gc;
    ValueCreator valueCreator(&gc);
    Value* val = valueCreator.create(13.0);

    //! [0]
    gc.addRoot(val);
    //! [0]

    trigger::ValueHandle valHandle = valueCreator.valueHandle(val);

    std::cout << "Value Handle :[" << valHandle.get() << "]" << std::endl;

    valHandle->print();

    std::cout << std::endl;

    gc.connectionManager()->printConnections();

    gc.collect(true);

    std::cout << "Value Handle :[" << valHandle.get() << "]" << std::endl;

    gc.connectionManager()->printConnections();

    return 0;
}

