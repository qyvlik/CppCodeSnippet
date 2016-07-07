//! [code from](https://github.com/doublec/gc)

#include "gc.h"

#include <iostream>
#include <vector>
#include <cassert>

#include <sys/time.h>

using namespace std;

namespace qyvlik {
namespace gc {


// GCObject
bool GCObject::isMarked() const
{
    return mMarked;
}

void GCObject::setMarked(bool marked)
{
    mMarked = marked;
}

GCObject::GCObject() :
    mMarked(false)
{
    // GarbageCollector::GC.addObject(this);
}


GCObject::~GCObject()
{
    // cout << "destroy: [" << this  << "]" << endl;
}

void GCObject::mark()
{
    if (!mMarked) {
        mMarked = true;
        markChildren();
    }
}

void GCObject::markChildren()
{
}


// GarbageCollector
GarbageCollector GarbageCollector::GC;

void GarbageCollector::collect(bool verbose)
{
    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);

    // Mark root objects
    for (ObjectSet::iterator it = mRoots.begin();
         it != mRoots.end();
         ++it)
        (*it)->mark();

    // Mark pinned objects
    for (PinnedSet::iterator it = mPinned.begin();
         it != mPinned.end();
         ++it)
        (*it).first->mark();

    if (verbose) {
        cout << "Roots: " << mRoots.size() << endl;
        cout << "Pinned: " << mPinned.size() << endl;
        cout << "GC: " << mHeap.size() << " objects in heap" << endl;
    }

    sweep(verbose);

    gettimeofday(&tv_end, NULL);

    if (verbose) {
        cout << "GC: " << (tv_end.tv_sec -tv_begin.tv_sec) << " seconds, " << (tv_end.tv_usec-tv_begin.tv_usec) << " milliseconds" << endl;
    }
}

void GarbageCollector::sweep(bool verbose)
{
    unsigned int live = 0;
    unsigned int dead = 0;
    unsigned int total = 0;
    vector<ObjectSet::iterator> erase;
    for (ObjectSet::iterator it = mHeap.begin();
         it != mHeap.end();
         ++it) {
        GCObject* p = *it;
        total++;
        if (p->isMarked()) {
            p->setMarked(false);
            ++live;
        } else {
            erase.push_back(it);
        }
    }
    dead = erase.size();

    //    struct timeval tv_begin, tv_end;
    //    gettimeofday(&tv_begin, NULL);

    for (vector<ObjectSet::iterator>::iterator it = erase.begin();
         it != erase.end();
         ++it) {
        GCObject* p = **it;
        mHeap.erase(*it);
        delete p;
    }
    //    gettimeofday(&tv_end, NULL);
    //    cout << "delete: " << (tv_end.tv_sec -tv_begin.tv_sec) << " seconds, " << (tv_end.tv_usec-tv_begin.tv_usec) << " milliseconds" << endl;

    if (verbose) {
        cout << "GC: " << live << " objects live after sweep" << endl;
        cout << "GC: " << dead << " objects dead after sweep" << endl;
    }
}

void GarbageCollector::addRoot(GCObject* root)
{
    mRoots.insert(root);
}

void GarbageCollector::removeRoot(GCObject* root)
{
    mRoots.erase(root);
}

void GarbageCollector::pin(GCObject* o)
{
    PinnedSet::iterator it = mPinned.find(o);
    if (it == mPinned.end()) {
        mPinned.insert(make_pair(o, 1));
    }
    else {
        (*it).second++;
    }
}

void GarbageCollector::unpin(GCObject* o)
{
    PinnedSet::iterator it = mPinned.find(o);
    assert(it != mPinned.end());

    if (--((*it).second) == 0)
        mPinned.erase(it);
}

bool GarbageCollector::isPinObject(GCObject *o)
{
    return mPinned.find(o) != mPinned.end();
}

void GarbageCollector::addObject(GCObject* o)
{
    mHeap.insert(o);
}

void GarbageCollector::removeObject(GCObject* o)
{
    mHeap.erase(o);
}

int GarbageCollector::live()
{
    return mHeap.size();
}

}
}
