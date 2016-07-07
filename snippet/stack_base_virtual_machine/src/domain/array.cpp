#include "array.h"
#include "value.h"

namespace qyvlik {
namespace domain {

Array::Array():
    GCObject()
{

}

Array::~Array()
{}

void Array::markChildren()
{
    auto iter = mElements.begin();
    auto end  = mElements.end();
    while(iter != end ) {

        (*iter)->mark();

        iter++;
    }
}

int Array::size() const
{
    return mElements.size();
}

Value *Array::at(size_t index)
{
    return mElements.at(index);
}

const Value *Array::at(size_t index) const
{
    return mElements.at(index);
}

void Array::push(Value *val)
{
    mElements.push_back(val);
}

Value *Array::pop()
{
    if(!mElements.empty()) {
        Value* popItem = mElements.back();
        mElements.pop_back();
        return popItem;
    } else {
        return nullptr;
    }
}

void Array::unshift(Value *val)
{
    mElements.insert(mElements.begin(), val);
}

Value *Array::shift()
{
    if(!mElements.empty()) {
        Value* value = *(mElements.begin());
        mElements.erase(mElements.begin());
        return value;
    } else {
        return nullptr;
    }
}

int Array::insert(size_t index, Value *val)
{
    if(mElements.begin() + index < mElements.end()) {
        mElements.insert(mElements.begin() + index, val);
        return mElements.size();
    } else {
        return -1;
    }
}

Value *Array::remove(size_t index)
{
    if(!mElements.empty() && mElements.begin() +index < mElements.end()) {
        Value* value = *(mElements.begin()+index);
        mElements.erase(mElements.begin()+index);
        return value;
    } else {
        return nullptr;
    }
}

//-----------------------------------------------------

ArrayCreator::ArrayCreator(gc::GarbageCollector *gc):
    mGC(gc)
{}

ArrayCreator::~ArrayCreator()
{}

Array *ArrayCreator::create()
{
    Array* array = new Array;
    mGC->addObject(array);
    return array;
}

} // namespace domain
} // namespace qyvlik

