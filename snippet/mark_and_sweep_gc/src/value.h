#ifndef QYVLIK_VALUE_H
#define QYVLIK_VALUE_H

#include "declara_forword.hpp"
#include "gc/gc.h"

namespace qyvlik {

class ValuePrivate;
class Value : public gc::GCObject
{
protected:
    explicit Value();
    explicit Value(bool val);
    explicit Value(int val);
    explicit Value(const std::string& val);
    explicit Value(Array* val);
    explicit Value(Object* val);
public:
    enum Type {
        Undefined = -1,
        Boolean,
        Integer,
        Double,
        String,
        Array,
        Object,
        Function
    };

    virtual ~Value();

    Type type() const;

    bool            toBoolean() const;
    int             toInteger() const;
    double          toDouble()  const;
    std::string     toString()  const;

    qyvlik::Array*          toArray();
    const qyvlik::Array*    toArray() const;

    qyvlik::Object*         toObject();
    const qyvlik::Object*   toObject() const;

    void assign(bool val);
    void assign(int val);
    void assign(double val);
    void assign(const std::string& val);
    void assign(qyvlik::Array* val);
    void assign(qyvlik::Object* val);

    void print() const;

protected:
    void markChildren() override;

private:
    friend class ValueCreator;
    ValuePrivate* d_ptr;
};

class ValueCreator
{
public:
    explicit ValueCreator(gc::GarbageCollector* gc);
    ~ValueCreator();

    Value* create();
    Value* create(bool val);
    Value* create(int val);
    Value* create(const std::string& val);
    Value* create(Array* val);
    Value* create(Object* val);

    void destroy(Value* val);

private:
    gc::GarbageCollector* mGC;
};



} // namespace qyvlik

#endif // QYVLIK_VALUE_H
