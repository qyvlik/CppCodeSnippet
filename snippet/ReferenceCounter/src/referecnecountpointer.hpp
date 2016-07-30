#ifndef QYVLIK_REFERECNECOUNTPOINTER_HPP
#define QYVLIK_REFERECNECOUNTPOINTER_HPP

#include <atomic>
#include <iostream>
#include <cassert>

namespace qyvlik {


namespace base
{
class ReferecneCountPointer
{
public:
    ReferecneCountPointer(std::atomic<long> * counter):
        mCounter(counter)
    {}
protected:
    std::atomic<long> *mCounter;
};
}

template<typename T>
class ReferecneCountPointer : public base::ReferecneCountPointer
{
    static_assert(!std::is_pointer<T>::value,        "NOT SUPPORT POINTER");
    static_assert(!std::is_reference<T>::value,      "NOT SUPPORT REFERENCE");
    friend class CastHelper;
public:
    typedef T* TypePointer;

    friend class Helper;
    struct Helper
    {
        static std::atomic<long> *getCounter(ReferecneCountPointer& p) {
            return p.mCounter;
        }
    };

    ReferecneCountPointer():
        base::ReferecneCountPointer(nullptr),
        mPointer(nullptr)
    {}

    ReferecneCountPointer(TypePointer ptr):
        base::ReferecneCountPointer(new std::atomic<long>(1)),
        mPointer(ptr)
    {}

    ReferecneCountPointer(const ReferecneCountPointer& other):
        base::ReferecneCountPointer(other.mCounter),
        mPointer(other.mPointer)
    {
        mCounter && ++(*mCounter);
    }

    ReferecneCountPointer(ReferecneCountPointer&& other):
        base::ReferecneCountPointer(other.mCounter),
        mPointer(other.mPointer)
    {
        other.mCounter = nullptr;
        other.mPointer = nullptr;
    }

    ReferecneCountPointer(std::atomic<long>* counter, TypePointer pointer):
        base::ReferecneCountPointer(counter),
        mPointer(pointer)
    {
        mCounter && ++(*mCounter);
    }

    ~ReferecneCountPointer()
    {
        // 删除之前保存的引用
        if(this->mCounter && --(*mCounter) == 0) {
            delete mPointer;
            mPointer = nullptr;

            delete mCounter;
            mCounter = nullptr;
        }
    }

    ReferecneCountPointer& operator=(const ReferecneCountPointer& rhs)
    {
        if(this->mCounter == rhs.mCounter) return *this;

        if(rhs.mCounter != nullptr ) {
            ++(*(rhs.mCounter));
        }

        // 删除之前保存的引用
        if(this->mCounter && --(*mCounter) == 0) {
            delete mPointer;
            mPointer = nullptr;

            delete mCounter;
            mCounter = nullptr;
        }

        mPointer = rhs.mPointer;
        mCounter = rhs.mCounter;

        return *this;
    }

    inline bool operator ==(const ReferecneCountPointer& rhs)
    {
        return this->mPointer == rhs.mPointer;
    }

    inline bool operator ==(const TypePointer rhs)
    {
        return this->mPointer == rhs;
    }

    inline bool operator!=(const ReferecneCountPointer& rhs)
    {
        return this->mPointer != rhs.mPointer;
    }

    inline bool operator !=(const TypePointer rhs)
    {
        return this->mPointer != rhs;
    }

    inline bool operator<(const ReferecneCountPointer& rhs)
    {
        return this->mPointer < rhs.mPointer;
    }

    inline bool operator<(const TypePointer rhs)
    {
        return this->mPointer < rhs;
    }

    inline bool operator<=(const ReferecneCountPointer& rhs)
    {
        return this->mPointer <= rhs.mPointer;
    }

    inline bool operator<=(const TypePointer rhs)
    {
        return this->mPointer <= rhs;
    }

    inline bool operator>(const ReferecneCountPointer& rhs)
    {
        return this->mPointer > rhs.mPointer;
    }

    inline bool operator>(const TypePointer rhs)
    {
        return this->mPointer > rhs;
    }

    inline bool operator>=(const ReferecneCountPointer& rhs)
    {
        return this->mPointer >= rhs.mPointer;
    }

    inline bool operator>=(const TypePointer rhs)
    {
        return this->mPointer >= rhs;
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

    inline TypePointer get() const
    {
        return mPointer;
    }

    template<typename Type>
    ReferecneCountPointer<Type> cast()
    {
        Type* cast_pointer = dynamic_cast<Type*>(mPointer);
        return cast_pointer
                ? ReferecneCountPointer<Type>(this->mCounter, cast_pointer)
                : ReferecneCountPointer<Type>();
    }

    inline operator bool() const
    {
        return mPointer != nullptr;
    }

private:
    TypePointer mPointer;
};


} // namespace qyvlik

#endif // QYVLIK_REFERECNECOUNTPOINTER_HPP

