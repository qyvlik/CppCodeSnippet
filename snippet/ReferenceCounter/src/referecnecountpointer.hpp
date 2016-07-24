#ifndef QYVLIK_REFERECNECOUNTPOINTER_HPP
#define QYVLIK_REFERECNECOUNTPOINTER_HPP

#include <atomic>
#include <iostream>
#include <cassert>

namespace qyvlik {


template<typename T>
struct DefaultDeleter
{
public:
    void destroy(T* obj) {
        delete obj;
    }
};

template<typename T, typename Deleter = DefaultDeleter<T> >
class ReferecneCountPointer
{
    static_assert(!std::is_pointer<T>::value,        "NOT SUPPORT POINTER");
    static_assert(!std::is_reference<T>::value,      "NOT SUPPORT REFERENCE");

public:
    typedef T* TypePointer;

    ReferecneCountPointer():
        mCounter(nullptr),
        mPointer(nullptr)
    {}

    ReferecneCountPointer(TypePointer ptr):
        mCounter(new std::atomic<long>(1)),
        mPointer(ptr)
    {}

    ReferecneCountPointer(const ReferecneCountPointer& other):
        mCounter(other.mCounter),
        mPointer(other.mPointer)
    {
        mCounter && ++(*mCounter);
    }

    ReferecneCountPointer(ReferecneCountPointer&& other):
        mCounter(other.mCounter),
        mPointer(other.mPointer)
    {
        other.mCounter = nullptr;
        other.mPointer = nullptr;
    }

    ReferecneCountPointer(std::atomic<long>* count, TypePointer pointer):
        mCounter(count),
        mPointer(pointer)
    {
        mCounter && ++(*mCounter);
    }

    ~ReferecneCountPointer()
    {
        // 删除之前保存的引用
        if(this->mCounter && --(*mCounter) == 0) {
            Deleter(). destroy(mPointer);
            mPointer = nullptr;

            delete mCounter;
            mCounter = nullptr;
        }
    }

    ReferecneCountPointer& operator=(const ReferecneCountPointer& rhs)
    {
        // 证同是否必要？
        // if(this == &rhs || this->mCounter == rhs.mCounter) return *this;

        if(rhs.mCounter != nullptr ) {
            ++(*(rhs.mCounter));
        }

        // 删除之前保存的引用
        if(this->mCounter && --(*mCounter) == 0) {
            Deleter().destroy(mPointer);
            mPointer = nullptr;

            delete mCounter;
            mCounter = nullptr;
        }

        mPointer = rhs.mPointer;
        mCounter = rhs.mCounter;

        return *this;
    }

    inline long useCount() const
    {
        return mCounter ? (long)(*mCounter) : -1;
    }

    inline TypePointer operator->() const
    {
        assert(mPointer != nullptr);
        return mPointer;
    }

    inline TypePointer get() const {
        return mPointer;
    }

    template<typename Type>
    ReferecneCountPointer<Type> cast()
    {
        Type* cast_pointer = dynamic_cast<Type*>(mPointer);
        return cast_pointer
                ? ReferecneCountPointer<Type>(mCounter, cast_pointer)
                : ReferecneCountPointer<Type>();
    }

    inline operator bool() const {
        return mPointer != nullptr;
    }

private:
    std::atomic<long> *mCounter;
    TypePointer mPointer;
};


template<typename Type>
class SafeShareable
{
public:
    class PointerGetter
    {
        friend class SafeShareable<Type>;
        friend class ReferecneCountPointer<PointerGetter>;
    public:
        inline Type* getRawPointer() const {
            return mPtr;
        }
    protected:
        PointerGetter():
            mPtr(nullptr)
        {}
        PointerGetter(Type* ptr):
            mPtr(ptr)
        {}
        Type* mPtr;
    };

public:
    typedef ReferecneCountPointer<PointerGetter> ShareablePointerGetter;

    SafeShareable(Type* ptr):
        mPointerGetter(new PointerGetter(ptr))
    {
    }

    virtual ~SafeShareable()
    {}

    //@interface
    inline ReferecneCountPointer<PointerGetter> getPointerGetter() const {
        return mPointerGetter;
    }

    inline void onDestroy() {
        mPointerGetter->mPtr = nullptr;
    }

    inline void onInitialized(Type* ptr) {
        mPointerGetter->mPtr = ptr;
    }

protected:
    ReferecneCountPointer<PointerGetter> mPointerGetter;
};


} // namespace qyvlik

#endif // QYVLIK_REFERECNECOUNTPOINTER_HPP

