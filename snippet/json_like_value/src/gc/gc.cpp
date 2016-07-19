//! [code from](https://github.com/doublec/gc)

#include "gc.h"

#include <iostream>
#include <vector>
#include <cassert>

#include <sys/time.h>

#include "trigger.h"

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

trigger::OnDestroy *GCObject::onDestroy() const
{
    return mOnDestroy;
}

GCObject::GCObject() :
    mMarked(false),
    mOnDestroy(nullptr)
{
    // GarbageCollector::GC.addObject(this);
}


GCObject::~GCObject()
{
    // cout << "destroy: [" << this  << "]" << endl;
   if(mOnDestroy) {
       mOnDestroy->trigger();
       delete mOnDestroy;
   }

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

GarbageCollector::GarbageCollector():
    mConnectionManager(new trigger::ConnectionManager)
{}

GarbageCollector::~GarbageCollector()
{
    delete mConnectionManager;
}

void GarbageCollector::collect(bool verbose)
{
    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);

    // Mark root objects
    for (ObjectSet::iterator it = mRoots.begin();
         it != mRoots.end();
         ++it)
        (*it)->mark();

    if (verbose) {
        cout << "Roots: " << mRoots.size() << endl;
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

void GarbageCollector::decorateGCObject(GCObject *o)
{
    o->mOnDestroy = new trigger::OnDestroy(mConnectionManager);
}

}
}
