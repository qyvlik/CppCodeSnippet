#ifndef OBJECTPOOL_HPP
#define OBJECTPOOL_HPP

#include <type_traits>

#include <memory>
#include <vector>

namespace qyvlik {

class Reusable
{
public:
    virtual ~Reusable()
    {}
    virtual void init() = 0;
    virtual void clear() = 0;
};

template<typename T>
class ObjectPool
{
    static_assert(std::is_base_of<Reusable, T>::value, "Only Support Reusable");
    static_assert(std::is_default_constructible<T>::value, "Only Support The Object Which Has Default Construction!");
public:

    enum {
        ObjectSize = sizeof(T),
        ObjectCount = 64,
        ClearCount = ObjectCount * 2,
    };

    virtual ~ObjectPool()
    { }

    void fillLoop() {
        int time = ObjectCount;
        while(time-- != 0) {
            objectpool.push_back( std::shared_ptr<T> (new T()));
        }
    }

    size_t loopSize() const {
        return objectpool.size();
    }

    std::shared_ptr<T> getOne() {
        auto iter = objectpool.begin();
        auto end = objectpool.end();
        while(iter != end) {
            if((*iter).unique()) {

                (*iter)->clear();
                (*iter)->init();
                return (*iter);
            }
            iter++;
        }

        std::shared_ptr<T> ret(new T());

        if(objectpool.size() > ClearCount) {
            objectpool.clear();
        }
        objectpool.push_back(ret);
        return ret;
    }

private:
    bool avoid_fail;
    std::vector< std::shared_ptr<T> > objectpool;
};


}

#endif // OBJECTPOOL_HPP

