#ifndef THREADSTATICSINGLETON_HPP
#define THREADSTATICSINGLETON_HPP

#include <type_traits>

namespace qyvlik {

template<typename T>
class ThreadStaticSingleton
{
    static_assert(std::is_default_constructible<T>::value, "Only Support The Object Which Has Default Constructible!");
public:
    inline static T& getThreadstaticSingleton() {
        return singleton;
    }
private:
    thread_local static T singleton;
};

template<typename T> thread_local T ThreadStaticSingleton<T>::singleton;

}

#endif // THREADSTATICSINGLETON_HPP

