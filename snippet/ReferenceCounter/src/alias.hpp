#ifndef QYVLIK_ALIAS_H
#define QYVLIK_ALIAS_H

#include "referecnecountpointer.hpp"

#include <map>

namespace qyvlik {

template<typename Type>
class Alias
{
    struct PointerGetter{
        friend class Alias<Type>;
        PointerGetter(Type* ptr):
            mPtr(ptr)
        {}
        Type* mPtr;
    };
public:
    Alias():
        mPointerGetter(new PointerGetter(nullptr))
    {}

    Alias(Type* ptr):
        mPointerGetter(new PointerGetter(ptr))
    {}

    Alias(Alias&& alias) :
        mPointerGetter(alias.mPointerGetter)
    {}

    Alias(const Alias& alias) :
        mPointerGetter(alias.mPointerGetter)
    {}

    Alias& operator=(const Alias& rhs)
    {
        mPointerGetter = rhs.mPointerGetter;
        return *this;
    }

    inline bool operator==(const Alias& rhs)
    {
        return mPointerGetter == rhs.mPointerGetter;
    }

    inline bool operator!=(const Alias& rhs)
    {
        return mPointerGetter != rhs.mPointerGetter;
    }

    inline bool operator<(const Alias& rhs)
    {
        return mPointerGetter < rhs.mPointerGetter;
    }

    inline bool operator<=(const Alias& rhs)
    {
        return mPointerGetter <= rhs.mPointerGetter;
    }

    inline bool operator>(const Alias& rhs)
    {
        return mPointerGetter > rhs.mPointerGetter;
    }

    inline bool operator>=(const Alias& rhs)
    {
        return mPointerGetter >= rhs.mPointerGetter;
    }

    inline void onPointerDestroy()
    {
        mPointerGetter->mPtr = nullptr;
    }

    inline void onPointerInitialize(Type* ptr)
    {
        mPointerGetter->mPtr = ptr;
    }

    inline Type* operator->()const
    {
        assert(mPointerGetter->mPtr != nullptr);
        return mPointerGetter->mPtr;
    }

    inline Type* pointer() const
    {
        return mPointerGetter->mPtr;
    }

    inline long userCount() const
    {
        return mPointerGetter.useCount();
    }

    inline operator bool()
    {
        return mPointerGetter->mPtr != nullptr;
    }

private:
    ReferecneCountPointer<PointerGetter> mPointerGetter;
};


namespace base {
template<typename ObjectType>
class ObjectCreator
{
public:
    ObjectCreator() = default;

    virtual ~ObjectCreator()
    {}

    template<typename ... Args>
    Alias<ObjectType> create(Args... args) {
        ObjectType* object = new ObjectType(std::forward(args)...);
        Alias<ObjectType> alias(object);
        addObject(alias);
        return alias;
    }

   bool destroy(Alias<ObjectType>& alias) {
        if(removeObject(alias)) {
            destroyObject(alias.pointer());
            return true;
        }
        return false;
    }

protected:
    virtual void addObject(Alias<ObjectType>& alias) = 0;
    virtual bool removeObject(Alias<ObjectType>& alias) = 0;
    virtual void destroyObject(ObjectType* object) {
        delete object;
    }
};
}

template<typename ObjectType>
class ObjectCreator : public base::ObjectCreator<ObjectType>
{
public:
    ObjectCreator() = default;

protected:
    void addObject(Alias<ObjectType> &alias) override
    {
        mObjects.insert(std::pair<ObjectType*, Alias<ObjectType>>(alias.pointer(), alias));
    }

    bool removeObject(Alias<ObjectType> &alias) override
    {
        auto find = mObjects.find(alias.pointer());
        auto end  = mObjects.end();
        if(find != end) {
            mObjects.erase(find);
            alias.onPointerDestroy();
            return true;
        }
        return false;
    }

private:
    std::map<ObjectType*, Alias<ObjectType>> mObjects;
};

} // namespace qyvlik

#endif // QYVLIK_ALIAS_H
