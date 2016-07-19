#ifndef QYVLIK_ARRAY_H
#define QYVLIK_ARRAY_H

#include "../declara_forword.hpp"
#include "../gc/gc.h"

namespace qyvlik {

namespace domain {

class Array : public gc::GCObject
{
protected:
    Array();
public:
    virtual ~Array();

    int size() const;

    Value* at(size_t index);

    const Value* at(size_t index) const;

    void push(Value* val);

    Value* pop();

    // 添加到头
    void unshift(Value* val);

    // 移除头
    Value* shift();

    // length , <= 0 error
    int insert(size_t index, Value* val);

    Value* remove(size_t index);

protected:
    void markChildren() override;

private:
    friend class ArrayCreator;
    std::vector<Value*> mElements;
};

class ArrayCreator
{
public:
    explicit ArrayCreator(gc::GarbageCollector* gc);
    ~ArrayCreator();

    Array* create();

private:
    gc::GarbageCollector* mGC;
};

} // namespace domain

} // namespace qyvlik

#endif // QYVLIK_ARRAY_H
