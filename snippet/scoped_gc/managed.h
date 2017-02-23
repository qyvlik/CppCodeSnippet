#ifndef QYVLIK_MANAGED_H
#define QYVLIK_MANAGED_H

#include <iostream>
#include <set>
#include <map>
#include <sys/time.h>

namespace qyvlik {

static int64_t currentTimeMillis() {
   struct timeval tv;
   gettimeofday(&tv, nullptr);
   return (int64_t)(tv.tv_sec )* 1000 + tv.tv_usec / 1000;
}

class Managed {
public:
    Managed():
        mMarked(false),
        mRef(0)
    {}

    virtual ~Managed()
    {}

    bool isMarked() const {
        return mMarked;
    }

    void mark() {
        if (!mMarked) {
            mMarked = true;
            markChildren();
        }
    }

    void setMarked(bool marked) {
        mMarked = marked;
    }

    size_t ref() const {
        return mRef;
    }

    void addRef() {
        mRef++;
    }

    void subRef() {
        mRef--;
    }

protected:
    virtual void markChildren() {
    }

private:
    bool mMarked;
    size_t mRef;
};


// Garbage Collector. Implements mark and sweep GC algorithm.
class GarbageCollector {
public:
    // Perform garbage collection. If 'verbose' is true then
    // GC stats will be printed to stdout.
    void collect(bool verbose = false);

    // Add a root object to the collector.
    void addRoot(Managed* root);

    // Remove a root object from the collector.
    void removeRoot(Managed* root);

    // Pin an object so it temporarily won't be collected.
    // Pinned objects are reference counted. Pinning it
    // increments the count. Unpinning it decrements it. When
    // the count is zero then the object can be collected.
    void pin(Managed* o);
    void unpin(Managed* o);

    // Add an heap allocated object to the collector.
    void addObject(Managed* o);

    // Remove a heap allocated object from the collector.
    void removeObject(Managed* o);

    // Go through all objects in the heap, unmarking the live
    // objects and destroying the unreferenced ones.
    void sweep(bool verbose);

    // Number of live objects in heap
    int live();

private:
    // A collection of all active heap objects.
    typedef std::set<Managed*> ObjectSet;
    ObjectSet mHeap;

    // Collection of objects that are scanned for garbage.
    ObjectSet mRoots;

    // Pinned objects
    typedef std::map<Managed*, unsigned int> PinnedSet;
    PinnedSet mPinned;
};

//class Primitive
//{
//};

template<typename Type>
class Primitive : public Managed
{
    static_assert(!std::is_class<Type>::value, "NOT SUPPORT CLASS");
public:
    Primitive():
        Managed(),
        mData()
    {}

    explicit Primitive(const Type& d):
        Managed(),
        mData(d)
    {}

    explicit Primitive(Type&& d):
        Managed(),
        mData(d)
    {}

    Type getData() const {
        return mData;
    }

    void setData(const Type& d) {
        mData = d;
    }

protected:
    Type mData;
};


template<typename Type>
class Collection : public Managed
{
    static_assert(!std::is_class<Type>::value, "ONLY CLASS");
    typedef typename Type::value_type value_type;
public:
    int size() const;
    bool isEmpty() const;
protected:

};

} // namespace qyvlik

#endif // QYVLIK_MANAGED_H
