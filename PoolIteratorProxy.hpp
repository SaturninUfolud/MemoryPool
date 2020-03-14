#ifndef POOLITERATORPROXY_H
#define POOLITERATORPROXY_H

#include "MemoryPool.hpp"

constexpr std::size_t _POOL_ITERATOR_SIZE = sizeof (Pool<void*>::Iterator<false>);

template<typename T,bool IsConst=false>
class PoolIteratorProxy
{
public:
    using PoolPtr = typename Pool<T>::template Iterator<IsConst>;

    PoolIteratorProxy()
    {
        new(buffer)PoolPtr();
    }
    PoolIteratorProxy(const PoolPtr & ptr)
    {
        new(buffer)PoolPtr(ptr);
    }

    ~PoolIteratorProxy()
    {
        reinterpret_cast<PoolPtr*>(buffer)->~PoolPtr();
    }

    operator PoolPtr&()
    {
        return *reinterpret_cast<PoolPtr*>(buffer);
    }
private:
    char buffer[_POOL_ITERATOR_SIZE];
};

#endif // POOLITERATORPROXY_H
