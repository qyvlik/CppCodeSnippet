#ifndef QYVLIK_VALUE_H
#define QYVLIK_VALUE_H

#include "../declara_forword.hpp"
#include "../gc/gc.h"

namespace qyvlik {
namespace domain {


class ValuePrivate;
class Value : public gc::GCObject
{
protected:
    explicit Value();
    explicit Value(bool val);
    explicit Value(int val);
    explicit Value(double val);
    explicit Value(const std::string& val);
    explicit Value(qyvlik::domain::Array* val);
    explicit Value(qyvlik::domain::Object* val);
    explicit Value(qyvlik::domain::Callable* val);
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

    qyvlik::domain::Array*          toArray();
    const qyvlik::domain::Array*    toArray() const;

    qyvlik::domain::Object*         toObject();
    const qyvlik::domain::Object*   toObject() const;

    qyvlik::domain::Callable*       toCallable();
    const qyvlik::domain::Callable* toCallable() const;

    void assign(bool val);
    void assign(int val);
    void assign(double val);
    void assign(const std::string& val);
    void assign(qyvlik::domain::Array* val);
    void assign(qyvlik::domain::Object* val);
    void assign(qyvlik::domain::Callable* val);

    void assign(qyvlik::domain::Value* val);

    void print() const;

    bool equals(const Value* val) const;

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
    Value* create(double val);
    Value* create(const std::string& val);
    Value* create(Array* val);
    Value* create(Object* val);
    Value* create(Callable* val);

private:
    gc::GarbageCollector* mGC;
};

} // namespace domain
} // namespace qyvlik

#endif // QYVLIK_VALUE_H
