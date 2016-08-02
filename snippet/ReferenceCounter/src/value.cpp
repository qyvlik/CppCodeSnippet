#include "value.hpp"

namespace qyvlik {

void Value::execute(const Arguments &args, Reference<Value> result, Reference<Value> error) {
    (void)args;
    (void)result;
    (void)error;
}


//--------------------------------NullValue------------------------------

int NullValue::type() const
{
    return Null;
}

bool NullValue::toBoolean() const
{
    return false;
}

int NullValue::toInteger() const
{
    return 0;
}

double NullValue::toDouble() const
{
    return 0;
}

std::string NullValue::toString() const
{
    return "nul";
}

Reference<ArrayValue> NullValue::toArray() const
{
    return Reference<ArrayValue>();
}

Reference<ObjectValue> NullValue::toObject() const
{
    return Reference<ObjectValue>();
}

bool NullValue::equals(Value *value) const
{
    return this == value;
}

bool NullValue::equals(const Reference<Value> &value) const
{
    return this == value.pointer();
}

Reference<Value> NullValue::at(int index) const
{
    (void)index;
    return null();
}

Reference<Value> NullValue::at(const std::string &key) const
{
    (void)key;
    return null();
}

Reference<Value> NullValue::null()
{
    static Reference<Value> n(&mNull);
    return n;
}

Reference<ArrayValue> NullValue::nullArray()
{
    static Reference<ArrayValue> n(&mNullArray);
    return n;
}

Reference<ObjectValue> NullValue::nullObject()
{
    static Reference<ObjectValue> n(&mNullObject);
    return n;
}

NullValue NullValue::mNull;
ArrayValue NullValue::mNullArray;
ObjectValue NullValue::mNullObject;

//--------------------------------BooleanValue------------------------------

BooleanValue::BooleanValue(bool data):
    mData(data)
{}

int BooleanValue::type() const
{
    return Boolean;
}

bool BooleanValue::toBoolean() const
{
    return mData;
}

int BooleanValue::toInteger() const
{
    return (int)mData;
}

double BooleanValue::toDouble() const
{
    return (double)mData;
}

std::string BooleanValue::toString() const
{
    return mData ? "true" : "false";
}

Reference<ArrayValue> BooleanValue::toArray() const
{
    return NullValue::nullArray();
}

Reference<ObjectValue> BooleanValue::toObject() const
{
    return NullValue::nullObject();
}

bool BooleanValue::equals(Value *value) const
{
    if(this == value) return true;
    return value && this->type() == value->type() ?
                this->toBoolean() == value->toBoolean()
              : false;
}

bool BooleanValue::equals(const Reference<Value> &value) const
{
    return this->equals(value.pointer());
}

Reference<Value> BooleanValue::at(int index) const
{
    (void)index;
    return NullValue::null();
}

Reference<Value> BooleanValue::at(const std::string &key) const
{
    (void)key;
    return NullValue::null();
}

//--------------------------------BooleanValue------------------------------


IntegerValue::IntegerValue(int data):
    mData(data)
{}

int IntegerValue::type() const
{
    return Integer;
}

bool IntegerValue::toBoolean() const
{
    return (bool)mData;
}

int IntegerValue::toInteger() const
{
    return mData;
}

double IntegerValue::toDouble() const
{
    return mData;
}

std::string IntegerValue::toString() const
{
    return std::to_string(mData);
}

Reference<ArrayValue> IntegerValue::toArray() const
{
    return NullValue::nullArray();
}

Reference<ObjectValue> IntegerValue::toObject() const
{
    return NullValue::nullObject();
}

bool IntegerValue::equals(Value *value) const
{
    if(this == value) return true;
    return value && this->type() == value->type() ?
                this->toInteger() == value->toInteger()
              : false;
}

bool IntegerValue::equals(const Reference<Value> &value) const
{
    return this->equals(value.pointer());
}

Reference<Value> IntegerValue::at(int index) const
{
    (void)index;
    return NullValue::null();
}

Reference<Value> IntegerValue::at(const std::string &key) const
{
    (void)key;
    return NullValue::null();
}


//--------------------------------DoubleValue------------------------------

DoubleValue::DoubleValue(double data):
    mData(data)
{}

int DoubleValue::type() const
{
    return Double;
}

bool DoubleValue::toBoolean() const
{
    return mData != 0;
}

int DoubleValue::toInteger() const
{
    return mData;
}

double DoubleValue::toDouble() const
{
    return mData;
}

std::string DoubleValue::toString() const
{
    return std::to_string(mData);
}

Reference<ArrayValue> DoubleValue::toArray() const
{
    return NullValue::nullArray();
}

Reference<ObjectValue> DoubleValue::toObject() const
{
    return NullValue::nullObject();
}

bool DoubleValue::equals(Value *value) const
{
    if(this == value) return true;
    return value && this->type() == value->type() ?
                this->toDouble() == value->toDouble()
              : false;
}

bool DoubleValue::equals(const Reference<Value> &value) const
{
    return this->equals(value.pointer());
}

Reference<Value> DoubleValue::at(int index) const
{
    (void)index;
    return NullValue::null();
}

Reference<Value> DoubleValue::at(const std::string &key) const
{
    (void)key;
    return NullValue::null();
}


//--------------------------------DoubleValue------------------------------

StringValue::StringValue(const std::string &data):
    mData(data)
{}

int StringValue::type() const
{
    return String;
}

bool StringValue::toBoolean() const
{
    return !mData.empty();
}

int StringValue::toInteger() const
{
    return 0;
}

double StringValue::toDouble() const
{
    return 0;
}

std::string StringValue::toString() const
{
    return mData;
}

Reference<ArrayValue> StringValue::toArray() const
{
    return NullValue::nullArray();
}

Reference<ObjectValue> StringValue::toObject() const
{
    return NullValue::nullObject();
}

bool StringValue::equals(Value *value) const
{
    if(this == value) return true;
    return value && this->type() == value->type() ?
                this->toString() == value->toString()
              : false;
}

bool StringValue::equals(const Reference<Value> &value) const
{
    return this->equals(value.pointer());
}

Reference<Value> StringValue::at(int index) const
{
    (void)index;
    return NullValue::null();
}

Reference<Value> StringValue::at(const std::string &key) const
{
    (void)key;
    return NullValue::null();
}

//--------------------------------ObjectValue------------------------------

ObjectValue::ObjectValue()
{
    mReference.onPointerInitialize(this);
}

ObjectValue::~ObjectValue()
{
    mReference.onPointerDestroy();
}

int ObjectValue::type() const
{
    return Object;
}

bool ObjectValue::toBoolean() const
{
    return true;
}

int ObjectValue::toInteger() const
{
    return 0;
}

double ObjectValue::toDouble() const
{
    return 0;
}

std::string ObjectValue::toString() const
{
    return "Object["+addressToString(this) + "]";
}

Reference<ArrayValue> ObjectValue::toArray() const
{
    return NullValue::nullArray();
}

Reference<ObjectValue> ObjectValue::toObject() const
{
    return mReference;
}

bool ObjectValue::equals(Value *value) const
{
    // TODO
    if(this == value) return true;
    return false;
}

bool ObjectValue::equals(const Reference<Value> &value) const
{
    return this->equals(value.pointer());
}

Reference<Value> ObjectValue::at(int index) const
{
    (void)index;
    return NullValue::null();
}

Reference<Value> ObjectValue::at(const std::string &key) const
{
    auto find = mData.find(key);
    auto end  = mData.end();
    return find != end ? (*find).second : NullValue::null();
}


//--------------------------------ArrayValue------------------------------

ArrayValue::ArrayValue()
{
    mReference.onPointerInitialize(this);
}

ArrayValue::~ArrayValue()
{
    mReference.onPointerDestroy();
}

int ArrayValue::type() const
{
    return Array;
}

bool ArrayValue::toBoolean() const
{
    return false;
}

int ArrayValue::toInteger() const
{
    return 0;
}

double ArrayValue::toDouble() const
{
    return 0;
}

std::string ArrayValue::toString() const
{
    return "Array["+addressToString(this) + "]";
}

Reference<ArrayValue> ArrayValue::toArray() const
{
    return mReference;
}

Reference<ObjectValue> ArrayValue::toObject() const
{
    return NullValue::nullObject();
}

bool ArrayValue::equals(Value *value) const
{
    // TODO
    if(this == value) return true;
    return false;
}

bool ArrayValue::equals(const Reference<Value> &value) const
{
    return this->equals(value.pointer());
}

Reference<Value> ArrayValue::at(int index) const
{
    return mData.size() > index ? mData.at(index) : NullValue::null();
}

Reference<Value> ArrayValue::at(const std::string &key) const
{
    (void)key;
    return NullValue::null();
}

//--------------------------------ArrayValue------------------------------

FunctionValue::FunctionValue(const FunctionValue::FunctionType &f):
    mFunction(f)
{}

FunctionValue::~FunctionValue()
{}

int FunctionValue::type() const
{
    return Function;
}

bool FunctionValue::toBoolean() const
{
    return 0;
}

int FunctionValue::toInteger() const
{
    return 0;
}

double FunctionValue::toDouble() const
{
    return 0;
}

std::string FunctionValue::toString() const
{
    return "Function[" + addressToString(this) + "]";
}

Reference<ArrayValue> FunctionValue::toArray() const
{
    return NullValue::nullArray();
}

Reference<ObjectValue> FunctionValue::toObject() const
{
    return NullValue::nullObject();
}

bool FunctionValue::equals(Value *value) const
{
    // TODO
    if(this == value) return true;
    return false;
}

bool FunctionValue::equals(const Reference<Value> &value) const
{
    return this->equals(value.pointer());
}

Reference<Value> FunctionValue::at(int index) const
{
    (void)index;
    return NullValue::null();
}

Reference<Value> FunctionValue::at(const std::string &key) const
{
    (void)key;
    return NullValue::null();
}

void FunctionValue::execute(const Arguments &args, Reference<Value> result, Reference<Value> error) {
    mFunction(args, result, error);
}


} // namespace qyvlik
