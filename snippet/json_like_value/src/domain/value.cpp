#include "value.h"
#include "array.h"
#include "object.h"
#include "callable.h"

#include "../gc/trigger.h"

namespace qyvlik {
namespace domain {

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
        type(Value::Function),
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

Value::Value(double val):
    GCObject(),
    d_ptr(new ValuePrivate(val))
{

}

Value::Value(const std::string &val):
    GCObject(),
    d_ptr(new ValuePrivate(val))
{

}

Value::Value(qyvlik::domain::Array *val):
    GCObject(),
    d_ptr(new ValuePrivate(val))
{

}

Value::Value(qyvlik::domain::Object *val):
    GCObject(),
    d_ptr(new ValuePrivate(val))
{

}

Value::Value(qyvlik::domain::Callable *val):
    GCObject(),
    d_ptr(new ValuePrivate(val))
{

}

Value::~Value()
{
    delete d_ptr;
    std::cout << "~Value: [" << this << "]" << std::endl;
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
        // return d_ptr->array_value != nullptr && d_ptr->array_value->size() != 0;
        return d_ptr->array_value != nullptr ;
    case Value::Object:
        return d_ptr->object_value != nullptr;
    case Value::Undefined:
    case Value::Function:
        return d_ptr->callable_value != nullptr;
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

qyvlik::domain::Array *Value::toArray()
{
    return d_ptr->type == Value::Array ? d_ptr->array_value : nullptr;
}

const qyvlik::domain::Array *Value::toArray() const
{    
    return d_ptr->type == Value::Array ? d_ptr->array_value : nullptr;
}

qyvlik::domain::Object *Value::toObject()
{
    return d_ptr->type == Value::Object ? d_ptr->object_value : nullptr;
}

const qyvlik::domain::Object *Value::toObject() const
{    
    return d_ptr->type == Value::Object ? d_ptr->object_value : nullptr;
}

qyvlik::domain::Callable *Value::toCallable()
{
    return d_ptr->type == Value::Function ? d_ptr->callable_value : nullptr;
}

const qyvlik::domain::Callable *Value::toCallable() const
{    
    return d_ptr->type == Value::Function ? d_ptr->callable_value : nullptr;
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

void Value::assign(qyvlik::domain::Array *val)
{
    d_ptr->type = Value::Array;
    d_ptr->array_value = val;
}

void Value::assign(qyvlik::domain::Object *val)
{
    d_ptr->type = Value::Object;
    d_ptr->object_value = val;
}

void Value::assign(qyvlik::domain::Callable *val)
{
    d_ptr->type = Value::Function;
    d_ptr->callable_value = val;
}

void Value::assign(Value *val)
{
    if(val == nullptr || this == val) return;

    this->d_ptr->type = val->d_ptr->type;

    switch(d_ptr->type)
    {
    case Value::Boolean:
        d_ptr->bool_value = val->d_ptr->bool_value;
        break;
    case Value::Integer:
        d_ptr->int_value = val->d_ptr->int_value;
        break;
    case Value::Double:
        d_ptr->double_value = val->d_ptr->double_value;
        break;
    case Value::String:
        d_ptr->string_value = val->d_ptr->string_value;
        break;

    case Value::Array:
        d_ptr->array_value = val->d_ptr->array_value;
        break;
    case Value::Object:
        d_ptr->object_value = val->d_ptr->object_value;
        break;
    case Value::Function:
        d_ptr->callable_value = val->d_ptr->callable_value;
        break;

    case Value::Undefined:
    default:
        break;
    }
}

void Value::print() const
{
    std::cout << this->toString();
}

bool Value::equals(const Value *val) const
{
    if(this != val) {
        if(d_ptr->type == val->d_ptr->type) {
            switch(d_ptr->type)
            {
            case Value::Boolean:
                return d_ptr->bool_value == val->d_ptr->bool_value;
            case Value::Integer:
                return d_ptr->int_value == val->d_ptr->int_value;
            case Value::Double:
                return d_ptr->double_value == val->d_ptr->double_value;
            case Value::String:
                return d_ptr->string_value == val->d_ptr->string_value;

                // TODO
            case Value::Array:
            case Value::Object:
            case Value::Function:

            default:
                return false;
            }
        }
    }
    return true;
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
    case Value::Function:
        if(d_ptr->callable_value != nullptr) {
            d_ptr->callable_value->mark();
        }
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
    mGC->decorateGCObject(value);
    mGC->addObject(value);
    return value;
}

Value *ValueCreator::create(bool val)
{
    Value* value = new Value(val);
    mGC->decorateGCObject(value);
    mGC->addObject(value);
    return value;
}

Value *ValueCreator::create(int val)
{
    Value* value = new Value(val);
    mGC->decorateGCObject(value);
    mGC->addObject(value);
    return value;
}

Value *ValueCreator::create(double val)
{
    Value* value = new Value(val);
    mGC->decorateGCObject(value);
    mGC->addObject(value);
    return value;
}

Value *ValueCreator::create(const std::string &val)
{
    Value* value = new Value(val);
    mGC->decorateGCObject(value);
    mGC->addObject(value);
    return value;
}

Value *ValueCreator::create(Array *val)
{
    Value* value = new Value(val);
    mGC->decorateGCObject(value);
    mGC->addObject(value);
    return value;
}

Value *ValueCreator::create(Object *val)
{
    Value* value = new Value(val);
    mGC->decorateGCObject(value);
    mGC->addObject(value);
    return value;
}

Value *ValueCreator::create(Callable *val)
{
    Value* value = new Value(val);
    mGC->decorateGCObject(value);
    mGC->addObject(value);
    return value;
}

trigger::ValueHandle ValueCreator::valueHandle(Value *val)
{
    trigger::ValueHandle valueHandle(mGC->connectionManager());
    valueHandle.set(val);
    return valueHandle;
}

} // namespace domain
} // namespace qyvlik

