#include "managed.h"

#include <iostream>
#include <vector>
#include <cassert>


using namespace std;

namespace qyvlik {

void GarbageCollector::collect(bool verbose)
{
    int64_t startTime = currentTimeMillis();

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

    int64_t endTime = currentTimeMillis();

    if (verbose) {
        cout << "GC: " << (endTime - startTime) << " milliseconds" << endl;
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
        Managed* p = *it;
        total++;
        if (p->isMarked()) {
            p->setMarked(false);
            ++live;
        } else {
            erase.push_back(it);
        }
    }
    dead = erase.size();

    int64_t startTime = currentTimeMillis();

    for (vector<ObjectSet::iterator>::iterator it = erase.begin();
         it != erase.end();
         ++it) {
        Managed* p = **it;
        mHeap.erase(*it);
        delete p;
    }

    int64_t endTime = currentTimeMillis();

    if (verbose) {
        cout << "delete: " << (endTime - startTime) << " milliseconds" << endl;
        cout << "GC: " << live << " objects live after sweep" << endl;
        cout << "GC: " << dead << " objects dead after sweep" << endl;
    }
}

void GarbageCollector::addRoot(Managed* root)
{
    mRoots.insert(root);
}

void GarbageCollector::removeRoot(Managed* root)
{
    mRoots.erase(root);
}

void GarbageCollector::pin(Managed* o)
{
    PinnedSet::iterator it = mPinned.find(o);
    if (it == mPinned.end()) {
        mPinned.insert(make_pair(o, 1));
    }
    else {
        (*it).second++;
    }
}

void GarbageCollector::unpin(Managed* o)
{
    PinnedSet::iterator it = mPinned.find(o);
    assert(it != mPinned.end());

    if (--((*it).second) == 0)
        mPinned.erase(it);
}

void GarbageCollector::addObject(Managed* o)
{
    mHeap.insert(o);
}

void GarbageCollector::removeObject(Managed* o)
{
    mHeap.erase(o);
}

int GarbageCollector::live()
{
    return mHeap.size();
}

} // namespace qyvlik
