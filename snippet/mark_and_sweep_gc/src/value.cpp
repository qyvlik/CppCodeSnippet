#include "value.h"
#include "array.h"
#include "object.h"

namespace qyvlik {

//class StringCreator;
//class String : public gc::GCObject
//{
//protected:
//    String(const std::string& val):
//        str(val)
//    { }
//public:
//    std::string str;
//private:
//    friend class StringCreator;
//};

//class StringCreator
//{
//public:
//    explicit StringCreator(gc::GarbageCollector* gc):
//        mGC(gc)
//    { }

//    String* create(const std::string& string = "")
//    {
//        String* str = new String(string);
//        mGC->addObject(str);
//        return str;
//    }

//private:
//    gc::GarbageCollector* mGC;
//};

//------------------------------------------------------------

class ValuePrivate
{
public:
    ValuePrivate():
        type(Value::Undefined),
        bool_value(false),
        int_value(0),
        double_value(0),
        string_value(""),
        array_value(nullptr),
        object_value(nullptr),
        callable_value(nullptr)
    {}

    ValuePrivate(bool val):
        type(Value::Boolean),
        bool_value(val),
        int_value(0),
        double_value(0),
        string_value(""),
        array_value(nullptr),
        object_value(nullptr),
        callable_value(nullptr)
    { }

    ValuePrivate(int val):
        type(Value::Integer),
        bool_value(false),
        int_value(val),
        double_value(0),
        string_value(""),
        array_value(nullptr),
        object_value(nullptr),
        callable_value(nullptr)
    { }

    ValuePrivate(double val):
        type(Value::Double),
        bool_value(false),
        int_value(0),
        double_value(val),
        string_value(""),
        array_value(nullptr),
        object_value(nullptr),
        callable_value(nullptr)
    { }

    ValuePrivate(const std::string& val):
        type(Value::String),
        bool_value(false),
        int_value(0),
        double_value(0),
        string_value(val),
        array_value(nullptr),
        object_value(nullptr),
        callable_value(nullptr)
    { }

    ValuePrivate(Array* val):
        type(Value::Array),
        bool_value(false),
        int_value(0),
        double_value(0),
        string_value(""),
        array_value(val),
        object_value(nullptr),
        callable_value(nullptr)
    { }

    ValuePrivate(Object* val):
        type(Value::Object),
        bool_value(false),
        int_value(0),
        double_value(0),
        string_value(""),
        array_value(nullptr),
        object_value(val),
        callable_value(nullptr)
    { }

    ValuePrivate(Callable* val):
        type(Value::Object),
        bool_value(false),
        int_value(0),
        double_value(0),
        string_value(""),
        array_value(nullptr),
        object_value(nullptr),
        callable_value(val)
    { }

    int type;
    bool bool_value;
    int  int_value;
    double double_value;

    std::string string_value;
    Array* array_value;
    Object* object_value;
    Callable* callable_value;
};

//------------------------------------------------------------

Value::Value():
    GCObject(),
    d_ptr(new ValuePrivate)
{ }

Value::Value(bool val):
    GCObject(),
    d_ptr(new ValuePrivate(val))
{

}

Value::Value(int val):
    GCObject(),
    d_ptr(new ValuePrivate(val))
{

}

Value::Value(const std::string &val):
    GCObject(),
    d_ptr(new ValuePrivate(val))
{

}

Value::Value(qyvlik::Array *val):
    GCObject(),
    d_ptr(new ValuePrivate(val))
{

}

Value::Value(qyvlik::Object *val):
    GCObject(),
    d_ptr(new ValuePrivate(val))
{

}

Value::Value(Callable *val):
    GCObject(),
    d_ptr(new ValuePrivate(val))
{

}

Value::~Value()
{
    delete d_ptr;
}

Value::Type Value::type() const
{
    return (Value::Type)d_ptr->type;
}

bool Value::toBoolean() const
{
    switch(d_ptr->type)
    {
    case Value::Boolean:
        return d_ptr->bool_value;
    case Value::Integer:
        return d_ptr->int_value != 0;
    case Value::Double:
        return d_ptr->double_value != 0;
    case Value::String:
        return d_ptr->string_value.size() != 0;
    case Value::Array:
        return d_ptr->array_value != nullptr && d_ptr->array_value->size() != 0;
    case Value::Object:
        return d_ptr->object_value != nullptr;
    case Value::Undefined:
    default:
        return false;
    }
}

int Value::toInteger() const
{
    switch(d_ptr->type)
    {
    case Value::Boolean:
        return d_ptr->bool_value;
    case Value::Integer:
        return d_ptr->int_value;
    case Value::Double:
        return d_ptr->double_value;
    case Value::String:
    case Value::Array:
    case Value::Object:
    case Value::Undefined:
    default:
        return -1;
    }
}

double Value::toDouble() const
{
    switch(d_ptr->type)
    {
    case Value::Boolean:
        return d_ptr->bool_value;
    case Value::Integer:
        return d_ptr->int_value;
    case Value::Double:
        return d_ptr->double_value;
    case Value::String:
    case Value::Array:
    case Value::Object:
    case Value::Undefined:
    default:
        return 0;
    }
}

std::string Value::toString() const
{
    switch(d_ptr->type)
    {
    case Value::Boolean:
        return d_ptr->bool_value ? "true": "false";
    case Value::Integer:
        return std::to_string(d_ptr->int_value);
    case Value::Double:
        return std::to_string(d_ptr->double_value);
    case Value::String:
        return d_ptr->string_value;
    case Value::Array:
        return "[Array]";
    case Value::Object:
        return "[Object]";
    case Value::Undefined:
    default:
        return "undefined";
    }
}

qyvlik::Array *Value::toArray()
{
    switch(d_ptr->type)
    {
    case Value::Array:
        return d_ptr->array_value;

    case Value::Boolean:

    case Value::Integer:

    case Value::Double:

    case Value::String:

    case Value::Object:

    case Value::Undefined:
    default:
        return nullptr;
    }
}

const qyvlik::Array *Value::toArray() const
{
    switch(d_ptr->type)
    {
    case Value::Array:
        return d_ptr->array_value;

    case Value::Boolean:

    case Value::Integer:

    case Value::Double:

    case Value::String:

    case Value::Object:

    case Value::Undefined:
    default:
        return nullptr;
    }
}

qyvlik::Object *Value::toObject()
{
    switch(d_ptr->type)
    {
    case Value::Object:
        return d_ptr->object_value;
    case Value::Array:

    case Value::Boolean:

    case Value::Integer:

    case Value::Double:

    case Value::String:

    case Value::Undefined:
    default:
        return nullptr;
    }
}

const qyvlik::Object *Value::toObject() const
{
    switch(d_ptr->type)
    {
    case Value::Object:
        return d_ptr->object_value;
    case Value::Array:

    case Value::Boolean:

    case Value::Integer:

    case Value::Double:

    case Value::String:


    case Value::Undefined:
    default:
        return nullptr;
    }
}

Callable *Value::toCallable()
{
    if(d_ptr->type == Value::Function) {
        return d_ptr->callable_value;
    } else {
        return nullptr;
    }
}

const Callable *Value::toCallable() const
{
    if(d_ptr->type == Value::Function) {
        return d_ptr->callable_value;
    } else {
        return nullptr;
    }
}

void Value::assign(bool val)
{
    d_ptr->type = Value::Boolean;
    d_ptr->bool_value = val;
}

void Value::assign(int val)
{
    d_ptr->type = Value::Integer;
    d_ptr->int_value = val;
}

void Value::assign(double val)
{
    d_ptr->type = Value::Double;
    d_ptr->double_value = val;
}

void Value::assign(const std::string &val)
{
    d_ptr->type = Value::String;
    d_ptr->string_value = val;
}

void Value::assign(qyvlik::Array *val)
{
    d_ptr->type = Value::Array;
    d_ptr->array_value = val;
}

void Value::assign(qyvlik::Object *val)
{
    d_ptr->type = Value::Object;
    d_ptr->object_value = val;
}

void Value::assign(qyvlik::Callable *val)
{
    d_ptr->type = Value::Function;
    d_ptr->callable_value = val;
}

void Value::print() const
{
    std::cout << this->toString();
}

void Value::markChildren()
{
    switch(d_ptr->type)
    {
    case Value::Boolean:

    case Value::Integer:

    case Value::Double:

    case Value::String:

    case Value::Undefined:

    case Value::Array:
        if( d_ptr->array_value != nullptr)
            d_ptr->array_value->mark();
        break;
    case Value::Object:
        if(d_ptr->object_value != nullptr )
            d_ptr->object_value->mark();
        break;

    default:
        break;
    }
}

//------------------------------------------------------------

ValueCreator::ValueCreator(gc::GarbageCollector *gc) :
    mGC(gc)
{ }

ValueCreator::~ValueCreator()
{ }

Value *ValueCreator::create()
{
    Value* value = new Value();
    mGC->addObject(value);
    return value;
}

Value *ValueCreator::create(bool val)
{
    Value* value = new Value(val);
    mGC->addObject(value);
    return value;
}

Value *ValueCreator::create(int val)
{
    Value* value = new Value(val);
    mGC->addObject(value);
    return value;
}

Value *ValueCreator::create(const std::string &val)
{
    Value* value = new Value(val);
    mGC->addObject(value);
    return value;
}

Value *ValueCreator::create(Array *val)
{
    Value* value = new Value(val);
    mGC->addObject(value);
    return value;
}

Value *ValueCreator::create(Object *val)
{
    Value* value = new Value(val);
    mGC->addObject(value);
    return value;
}

Value *ValueCreator::create(Callable *val)
{
    Value* value = new Value(val);
    mGC->addObject(value);
    return value;
}

void ValueCreator::destroy(Value *val)
{
    mGC->removeObject(val);
    mGC->removeRoot(val);
    delete val;
}



} // namespace qyvlik

