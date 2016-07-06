#include "object.h"
#include "value.h"

namespace qyvlik {

Object::~Object()
{ }

Object::Object():
    GCObject()
{}

void Object::markChildren()
{
    auto iter = this->mFields.begin();
    auto end = this->mFields.end();
    while(iter != end) {

        (*iter).second->mark();

        iter++;
    }
}

bool Object::setField(const std::string& index, Value *val)
{
    return mFields.insert(std::pair<std::string, Value*>(index, val)).second;
}

Value *Object::field(const std::string& index)
{
    auto find = mFields.find(index);
    return find != mFields.end() ? (*find).second : nullptr;
}

const Value *Object::field(const std::string& index) const
{
    auto find = mFields.find(index);
    return find != mFields.end() ? (*find).second : nullptr;
}

//-----------------------------------------------------

ObjectCreator::ObjectCreator(gc::GarbageCollector *gc):
    mGC(gc)
{}

ObjectCreator::~ObjectCreator()
{}

Object *ObjectCreator::create()
{
    Object* obj = new Object();
    mGC->addObject(obj);
    return obj;
}

} // namespace qyvlik

