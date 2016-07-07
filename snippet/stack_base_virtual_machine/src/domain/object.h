#ifndef QYVLIK_OBJECT_H
#define QYVLIK_OBJECT_H

#include "../declara_forword.hpp"
#include "../gc/gc.h"

namespace qyvlik {

namespace domain {

class Object : public gc::GCObject
{
protected:
    explicit Object();

public:
    virtual ~Object();

    bool setField(const std::string& index, Value* val);

    Value* field(const std::string& index);

    const Value* field(const std::string& index) const;

protected:
    void markChildren() override;

private:
    friend class ObjectCreator;
    std::map<std::string, Value*> mFields;
};

class ObjectCreator
{
public:
    explicit ObjectCreator(gc::GarbageCollector* gc);
    ~ObjectCreator();

    Object* create();

private:
    gc::GarbageCollector* mGC;
};

} // namespace domain
} // namespace qyvlik

#endif // QYVLIK_OBJECT_H
