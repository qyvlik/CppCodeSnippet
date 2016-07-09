#ifndef QYVLIK_REFERECNECOUNTPOINTER_HPP
#define QYVLIK_REFERECNECOUNTPOINTER_HPP

#include <atomic>
#include <iostream>

namespace qyvlik {

class ReferenceCounter
{
public:
    ReferenceCounter():
        mCount(1)
    { }

    long increase() {
        return ++mCount;
    }

    long decrease() {
        return --mCount;
    }

    long count() const {
        return mCount;
    }

private:
    std::atomic<long> mCount;
};

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
public:
    static_assert(!std::is_pointer<T>::value,        "NOT SUPPORT POINTER");
    static_assert(!std::is_reference<T>::value,      "NOT SUPPORT REFERENCE");

    typedef T* TypePointer;

    ReferecneCountPointer(TypePointer ptr):
        mCounter(new ReferenceCounter),
        mPointer(ptr)
    { }

    ReferecneCountPointer(const ReferecneCountPointer& other):
        mCounter(other.mCounter),
        mPointer(other.mPointer)
    {
        mCounter->increase();
    }

    ~ReferecneCountPointer()
    {
        // 删除之前保存的引用
        if(this->mCounter->decrease() == 0) {
            Deleter(). destroy(mPointer);
            mPointer = nullptr;

            delete mCounter;
            mCounter = nullptr;
        }
    }

    ReferecneCountPointer& operator=(const ReferecneCountPointer& rhs)
    {
        rhs.mCounter->increase();

        // 删除之前保存的引用
        if(this->mCounter->decrease() == 0) {
            Deleter().destroy(mPointer);
            mPointer = nullptr;

            delete mCounter;
            mCounter = nullptr;
        }
        mPointer = rhs.mPointer;
        mCounter = rhs.mCounter;
        return *this;
    }

    long useCount() const {
        return mCounter->count();
    }

    TypePointer operator->() const {
        return mPointer;
    }

private:
    ReferenceCounter* mCounter;
    TypePointer mPointer;
};


} // namespace qyvlik

#endif // QYVLIK_REFERECNECOUNTPOINTER_HPP

