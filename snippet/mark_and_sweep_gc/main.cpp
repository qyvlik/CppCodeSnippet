#include <iostream>

#include "src/value.h"
#include "src/array.h"
#include "src/object.h"

using namespace qyvlik;
using namespace qyvlik::gc;

int main()
{
    GarbageCollector gc;
    ValueCreator valueCreator(&gc);
    ArrayCreator arrayCreator(&gc);

    Array* array = arrayCreator.create();

    int size = 100000;
    while(size != 0) {

        array->push(valueCreator.create(size));

        size--;
    }

    gc.collect(true);

    return 0;
}

