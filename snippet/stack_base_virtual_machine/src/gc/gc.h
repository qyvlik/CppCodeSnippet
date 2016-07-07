//! [code from](https://github.com/doublec/gc)

#ifndef QYVLIK_GC_H
#define QYVLIK_GC_H

#include <set>
#include <map>

namespace qyvlik {
namespace gc {

// Base class for all objects that are tracked by
// the garbage collector.
class GCObject {
public:
    explicit GCObject();
    virtual ~GCObject();

    // Mark the object and all its children as live
    void mark();

    bool isMarked() const;
    void setMarked(bool marked);

protected:
    // Overridden by derived classes to call mark()
    // on objects referenced by this object. The default
    // implemention does nothing.
    virtual void markChildren();

private:
    // For mark and sweep algorithm. When a GC occurs
    // all live objects are traversed and mMarked is
    // set to true. This is followed by the sweep phase
    // where all unmarked objects are deleted.
    bool mMarked;
};

// Garbage Collector. Implements mark and sweep GC algorithm.
class GarbageCollector {
public:
    // Global garbage collector object
    static GarbageCollector GC;

    // Perform garbage collection. If 'verbose' is true then
    // GC stats will be printed to stdout.
    void collect(bool verbose = false);

    // Add a root object to the collector.
    void addRoot(GCObject* root);

    // Remove a root object from the collector.
    void removeRoot(GCObject* root);

    // Pin an object so it temporarily won't be collected.
    // Pinned objects are reference counted. Pinning it
    // increments the count. Unpinning it decrements it. When
    // the count is zero then the object can be collected.
    void pin(GCObject* o);
    void unpin(GCObject* o);
    bool isPinObject(GCObject* o);

    // Add an heap allocated object to the collector.
    void addObject(GCObject* o);

    // Remove a heap allocated object from the collector.
    void removeObject(GCObject* o);

    // Go through all objects in the heap, unmarking the live
    // objects and destroying the unreferenced ones.
    void sweep(bool verbose);

    // Number of live objects in heap
    int live();

private:
    // A collection of all active heap objects.
    typedef std::set<GCObject*> ObjectSet;
    ObjectSet mHeap;

    // Collection of objects that are scanned for garbage.
    ObjectSet mRoots;

    // Pinned objects
    typedef std::map<GCObject*, unsigned int> PinnedSet;
    PinnedSet mPinned;
};

}
}

#endif // QYVLIK_GC_H

