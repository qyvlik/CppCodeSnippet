#ifndef QYVLIK_VALUE_H
#define QYVLIK_VALUE_H

#include "reference.hpp"

#include <sstream>
#include <vector>
#include <string>
#include <functional>

namespace qyvlik {

class Value;
class NullValue;
class BooleanValue;
class IntegerValue;
class DoubleValue;
class StringValue;
class ArrayValue;
class ObjectValue;
class FunctionValue;

typedef std::vector<Reference<Value>> Arguments;

template<typename T>
inline std::string addressToString(const T* p)
{
    std::stringstream ss;
    ss << p;
    std::string str;
    ss >> str;
    return str;
}

class Value
{
public:
    enum ValueType {
        Null,
        Boolean,
        Integer,
        Double,
        String,
        Array,
        Object,
        Function,
    };

    Value() = default;
    virtual ~Value();
    virtual int type() const = 0;
    virtual bool toBoolean() const = 0;
    virtual int toInteger() const = 0;
    virtual double toDouble() const = 0;
    virtual std::string toString() const = 0;
    virtual Reference<ArrayValue> toArray() const = 0;
    virtual Reference<ObjectValue> toObject() const = 0;
    virtual bool equals(Value* value) const = 0;
    virtual bool equals(const Reference<Value>& value) const = 0;
    virtual Reference<Value> at(int index) const = 0;
    virtual Reference<Value> at(const std::string& key) const = 0;
    virtual void execute(const Arguments& args, Reference<Value> result, Reference<Value> error);

    virtual void assign(bool value);
    virtual void assign(int value);
    virtual void assign(double value);
    virtual void assign(const std::string& value);

    //    virtual Value* clone() const = 0;
};


class NullValue : public Value
{
public:
    int type() const override;
    bool toBoolean() const override;
    int toInteger() const override;
    double toDouble() const override;
    std::string toString() const override;
    Reference<ArrayValue> toArray() const override;
    Reference<ObjectValue> toObject() const override;
    bool equals(Value *value) const override;
    bool equals(const Reference<Value> &value) const override;
    Reference<Value> at(int index) const override;
    Reference<Value> at(const std::string &key) const override;
    void assign(bool value) override;
    void assign(int value) override;
    void assign(double value) override;
    void assign(const std::string &value) override;
    static Reference<Value> null();
    static Reference<ArrayValue> nullArray();
    static Reference<ObjectValue> nullObject();

private:
    static NullValue mNull;
    static ArrayValue mNullArray;
    static ObjectValue mNullObject;
};

class BooleanValue : public Value
{
public:
    BooleanValue(bool data);
    int type() const override;
    bool toBoolean() const override;
    int toInteger() const override;
    double toDouble() const override;
    std::string toString() const override;
    Reference<ArrayValue> toArray() const override;
    Reference<ObjectValue> toObject() const override;
    bool equals(Value *value) const override;
    bool equals(const Reference<Value> &value) const override;
    Reference<Value> at(int index) const override;
    Reference<Value> at(const std::string &key) const override;
    void assign(bool value) override;
    void assign(int value) override;
    void assign(double value) override;
    void assign(const std::string &value) override;
private:
    bool mData;
};

class IntegerValue : public Value
{
public:
    IntegerValue(int data);
    int type() const override;
    bool toBoolean() const override;
    int toInteger() const override;
    double toDouble() const override;
    std::string toString() const override;
    Reference<ArrayValue> toArray() const override;
    Reference<ObjectValue> toObject() const override;
    bool equals(Value *value) const override;
    bool equals(const Reference<Value> &value) const override;
    Reference<Value> at(int index) const override;
    Reference<Value> at(const std::string &key) const override;
    void assign(bool value) override;
    void assign(int value) override;
    void assign(double value) override;
    void assign(const std::string &value) override;
private:
    int mData;
};

class DoubleValue : public Value
{
public:
    DoubleValue(double data);
    int type() const override;
    bool toBoolean() const override;
    int toInteger() const override;
    double toDouble() const override;
    std::string toString() const override;
    Reference<ArrayValue> toArray() const override;
    Reference<ObjectValue> toObject() const override;
    bool equals(Value *value) const override;
    bool equals(const Reference<Value> &value) const override;
    Reference<Value> at(int index) const override;
    Reference<Value> at(const std::string &key) const override;
    void assign(bool value) override;
    void assign(int value) override;
    void assign(double value) override;
    void assign(const std::string &value) override;
private:
    double mData;
};

class StringValue : public Value
{
public:
    StringValue(const std::string& data);
    int type() const override;
    bool toBoolean() const override;
    int toInteger() const override;
    double toDouble() const override;
    std::string toString() const override;
    Reference<ArrayValue> toArray() const override;
    Reference<ObjectValue> toObject() const override;
    bool equals(Value *value) const override;
    bool equals(const Reference<Value> &value) const override;
    Reference<Value> at(int index) const override;
    Reference<Value> at(const std::string &key) const override;
    void assign(bool value) override;
    void assign(int value) override;
    void assign(double value) override;
    void assign(const std::string &value) override;
private:
    std::string mData;
};

class ObjectValue : public Value
{
public:
    ObjectValue();
    ~ObjectValue();
    int type() const override;
    bool toBoolean() const override;
    int toInteger() const override;
    double toDouble() const override;
    std::string toString() const override;
    Reference<ArrayValue> toArray() const override;
    Reference<ObjectValue> toObject() const override;
    bool equals(Value *value) const override;
    bool equals(const Reference<Value> &value) const override;
    Reference<Value> at(int index) const override;
    Reference<Value> at(const std::string &key) const override;
private:
    std::map<std::string, Reference<Value> > mData;
    Reference<ObjectValue> mReference;
};

class ArrayValue : public Value
{
public:
    ArrayValue();
    ~ArrayValue();
    int type() const override;
    bool toBoolean() const override;
    int toInteger() const override;
    double toDouble() const override;
    std::string toString() const override;
    Reference<ArrayValue> toArray() const override;
    Reference<ObjectValue> toObject() const override;
    bool equals(Value *value) const override;
    bool equals(const Reference<Value> &value) const override;
    Reference<Value> at(int index) const override;
    Reference<Value> at(const std::string &key) const override;
private:
    std::vector< Reference<Value> > mData;
    Reference<ArrayValue> mReference;
};

class FunctionValue : public Value
{
public:
    typedef std::function<void(const Arguments &args, Reference<Value> result, Reference<Value> error)> FunctionType;
    FunctionValue(const FunctionType& f);
    ~FunctionValue();
    int type() const override;
    bool toBoolean() const override;
    int toInteger() const override;
    double toDouble() const override;
    std::string toString() const override;
    Reference<ArrayValue> toArray() const override;
    Reference<ObjectValue> toObject() const override;
    bool equals(Value *value) const override;
    bool equals(const Reference<Value> &value) const override;
    Reference<Value> at(int index) const override;
    Reference<Value> at(const std::string &key) const override;
    void execute(const Arguments &args, Reference<Value> result, Reference<Value> error) override;
private:
    FunctionType mFunction;
};

} // namespace qyvlik

#endif // QYVLIK_VALUE_H
