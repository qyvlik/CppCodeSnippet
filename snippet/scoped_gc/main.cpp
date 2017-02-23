#include <iostream>
#include <vector>
#include <algorithm>

#include "managed.h"


#define RUN_COUNT(__COUNT__) for(int __count__ = 0; __count__ < __COUNT__; __count__ ++)

using namespace std;
using namespace qyvlik;

typedef Primitive<int32_t> IntValue;

class Stack : public Managed
{
public:
    Stack():
        Managed()
    {}

    void push(Managed* v) {
        v->addRef();
        mData.push_back(v);
    }

    Managed* pop() {
        Managed* p = mData.back();
        mData.pop_back();
        p->subRef();
        return p;
    }

    bool empty() const {
        return mData.empty();
    }

    size_t size() const {
        return mData.size();
    }

protected:
    vector<Managed*> mData;

    // Managed interface
protected:
    void markChildren() override
    {
        std::for_each(mData.begin(),
                      mData.end(),
                      [](Managed* m){
            m->mark();
        });
    }
};

class Scoped
{
public:
    Scoped(GarbageCollector* gc):
        mGC(gc)
    {}

    ~Scoped() {
        std::for_each(mInside.begin(),
                      mInside.end(),
                      [this](Managed* m){
            if(m->ref() == 0) {
                this->destory(m);
            }
        });
    }

    template<typename Type>
    Primitive<Type>* create(const Type& d) {
        Primitive<Type>* m = new Primitive<Type>(d);
        mGC->addObject(m);
        mInside.insert(m);
        return m;
    }

    void destory(Managed* m) {
        delete m;
        mGC->removeObject(m);
    }

private:
    GarbageCollector* mGC;
    set<Managed*> mInside;
};

int main(int argc, char *argv[])
{
    int64_t startTime = currentTimeMillis();

    GarbageCollector gc;

    Stack* s = new Stack();
    gc.addObject(s);

    {
        Scoped scoped(&gc);
        RUN_COUNT(999999) {
            IntValue* i = scoped.create(__count__);
            if(__count__ % 2 == 0) {
                s->push(i);
            }
        }
    }


    std::cout << "live:" << gc.live() << std::endl;

    gc.collect(true);

    int64_t endTime = currentTimeMillis();

    std::cout << "pass: " << (endTime - startTime) << "ms" << std::endl;

    return 0;
}








