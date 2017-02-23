#ifndef QYVLIK_REFERENCE_H
#define QYVLIK_REFERENCE_H

#include "referecnecountpointer.hpp"

#include <map>

namespace qyvlik {

template<typename Type>
class Reference
{
public:
    struct PointerGetter {
        PointerGetter(Type* ptr):
            mPtr(ptr)
        { }
        Type* mPtr;
    };

    Reference():
        mPointerGetter(new PointerGetter(nullptr))
    {}

    Reference(Type* ptr):
        mPointerGetter(new PointerGetter(ptr))
    {}

    Reference(Reference&& alias) :
        mPointerGetter(std::move(alias.mPointerGetter))
    {}

    Reference(const Reference& alias) :
        mPointerGetter(alias.mPointerGetter)
    {}

    Reference(ReferecneCountPointer<PointerGetter> rhs):
        mPointerGetter(rhs)
    {}

    Reference& operator=(const Reference& rhs)
    {
        mPointerGetter = rhs.mPointerGetter;
        return *this;
    }

    inline bool operator==(const Reference& rhs)
    {
        return mPointerGetter->mPtr == rhs.mPointerGetter->mPtr;
    }

    inline bool operator!=(const Reference& rhs)
    {
        return mPointerGetter->mPtr != rhs.mPointerGetter->mPtr;
    }

    inline bool operator<(const Reference& rhs)
    {
        return mPointerGetter->mPtr < rhs.mPointerGetter->mPtr;
    }

    inline bool operator<=(const Reference& rhs)
    {
        return mPointerGetter->mPtr <= rhs.mPointerGetter->mPtr;
    }

    inline bool operator>(const Reference& rhs)
    {
        return mPointerGetter->mPtr > rhs.mPointerGetter->mPtr;
    }

    inline bool operator>=(const Reference& rhs)
    {
        return mPointerGetter->mPtr >= rhs.mPointerGetter->mPtr;
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

    friend std::ostream& operator<<(std::ostream& os, const Reference& r) {
        os << "Reference [" << r.mPointerGetter->mPtr << "]";
        return os;
    }

    //! TODO cast

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
    Reference<ObjectType> create(Args&&... args) {
        ObjectType* object = new ObjectType(std::forward<Args>(args)...);
        Reference<ObjectType> alias(object);
        addObject(alias);
        return alias;
    }

    bool destroy(Reference<ObjectType>& alias) {
        if(removeObject(alias)) {
            destroyObject(alias.pointer());
            return true;
        }
        return false;
    }

protected:
    virtual void addObject(const Reference<ObjectType>& alias) = 0;
    virtual bool removeObject(Reference<ObjectType>& alias) = 0;
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
    void addObject(const Reference<ObjectType> &alias) override
    {
        mObjects.insert(std::pair<ObjectType*, Reference<ObjectType>>(alias.pointer(), alias));
    }

    bool removeObject(Reference<ObjectType> &alias) override
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
    std::map<ObjectType*, Reference<ObjectType>> mObjects;
};

} // namespace qyvlik

#endif // QYVLIK_REFERENCE_H
