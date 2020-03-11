#ifndef MEMORYPOOL_HPP
#define MEMORYPOOL_HPP
#include "MemoryPoolChunk.hpp"
#include <vector>
#include <stack>

template<typename T>
class Pool
{
public:
    Pool()
        :mSize(0)
    {

    }
    Pool(const Pool&source)
        :mChunks(source.mChunks), mSize(source.mSize)
    {

    }
    Pool(Pool&&source)
        :mChunks(std::move(source.mChunks)), mSize(source.mSize)
    {
        source.mSize=0;
    }
    ~Pool() = default;

    Pool& operator=(const Pool&source);
    Pool& operator=(Pool&&source);

    class Iterator
    {
    public:
        static Iterator nullIter;

        Iterator()
            :vecIndex(0),mChunksPtr(nullptr),chunkIter(typename PoolChunk<T>::Iterator())
        {

        }
        //static Iterator nullValue;

        Iterator& operator++();
        Iterator operator++(int);

        T& operator*()const
        {
            return *chunkIter;
        }

        T* operator->()const
        {
            return &(this->operator*());
        }

        bool operator==(const Iterator&it2)const
        {
            return this->chunkIter==it2;
        }
        bool operator!=(const Iterator&it2)const
        {
            return !(*this==it2);
        }

        operator bool()const
        {
            return (bool)this->chunkIter;
        }

        void setNull()
        {
            *this = nullIter;
        }

    private:
        friend class Pool;
        Iterator(size_t vecIndex,
                 std::vector<PoolChunk<T>>* mChunksPtr,
                 typename PoolChunk<T>::Iterator chunkIter)
        :vecIndex(vecIndex),mChunksPtr(mChunksPtr),chunkIter(chunkIter)
        {

        }

        std::size_t vecIndex;
        std::vector<PoolChunk<T>>* mChunksPtr;

        typename PoolChunk<T>::Iterator chunkIter;
    };

    Iterator begin()
    {
        return Iterator(0,&mChunks,mChunks[0].begin());
    }

    Iterator end()
    {
        return Iterator(mChunks.size(),&mChunks,mChunks[mChunks.size()-1].end());
    }

    Iterator insert(const T&item);
    Iterator insert(T&&item);

    void remove(Iterator& iter);

    Iterator copyIterator(Iterator oldIterator)
    {
        return Iterator(oldIterator.vecIndex,&mChunks,oldIterator.chunkIter);
    }


    class CIterator
    {
    public:
        CIterator()
        :vecIndex(0),mChunksPtr(nullptr),chunkIter(typename PoolChunk<T>::CIterator())
        {

        }
        //static Iterator nullValue;

        CIterator& operator++();
        CIterator operator++(int);

        const T& operator*()const
        {
            return *chunkIter;
        }

        const T* operator->()const
        {
            return &(this->operator*());
        }

        bool operator==(const CIterator&it2)const
        {
            return this->chunkIter==it2;
        }
        bool operator!=(const CIterator&it2)const
        {
            return !(*this==it2);
        }

        operator bool()const
        {
            return (bool)this->chunkIter;
        }

    private:
        friend class Pool;
        CIterator(size_t vecIndex,
                 const std::vector<PoolChunk<T>>* mChunksPtr,
                 typename PoolChunk<T>::CIterator chunkIter)
            :vecIndex(vecIndex),mChunksPtr(mChunksPtr),chunkIter(chunkIter)
        {

        }

        CIterator(const Iterator& iter)
            :vecIndex(iter.vecIndex),
              mChunksPtr(iter.mChunksPtr),
              chunkIter(iter.chunkIter)
        {

        }

        std::size_t vecIndex;
        const std::vector<PoolChunk<T>>* mChunksPtr;

        typename PoolChunk<T>::CIterator chunkIter;
    };

    CIterator begin()const
    {
        return CIterator(0,&mChunks,mChunks[0].begin());
    }

    CIterator end()const
    {
        return CIterator(mChunks.size(),&mChunks,mChunks[mChunks.size()-1].end());
    }

    std::size_t size()const
    {
        return this->mSize;
    }

private:
    std::vector<PoolChunk<T>> mChunks;

    std::size_t mSize;

    //TODO
    //std::stack<Iterator> mFreePositions;
    std::size_t reserveChunk();
};


template<typename T>
typename Pool<T>::Iterator Pool<T>::Iterator::nullIter;


template <typename T>
Pool<T>& Pool<T>::operator=(const Pool&source)
{
    if(this!=&source)
    {
        this->mSize = source.mSize;
        this->mChunks = source.mChunks;
    }
    return *this;
}

template <typename T>
Pool<T>& Pool<T>::operator=(Pool&&source)
{
    if(this!=&source)
    {
        this->mSize = source.mSize;
        this->mChunks = std::move(source.mChunks);

        source.mSize=0;
    }
    return *this;
}


template<typename T>
typename Pool<T>::Iterator Pool<T>::insert(const T&item)
{
    std::size_t vecIndex = this->reserveChunk();
    typename PoolChunk<T>::Iterator chunkIter = this->mChunks[vecIndex].insert(item);
    return Pool<T>::Iterator(vecIndex,&this->mChunks,chunkIter);
}

template<typename T>
typename Pool<T>::Iterator Pool<T>::insert(T&&item)
{
    std::size_t vecIndex = this->reserveChunk();
    typename PoolChunk<T>::Iterator chunkIter
            = this->mChunks[vecIndex].insert(std::move(item));
    return Pool<T>::Iterator(vecIndex,&this->mChunks,chunkIter);
}


template<typename T>
void Pool<T>::remove(Iterator&iter)
{    
    --this->mSize;
    this->mChunks[iter.vecIndex].remove(iter.chunkIter);
}

template<typename T>
std::size_t Pool<T>::reserveChunk()
{
    ++this->mSize;

    std::size_t i = this->mChunks.size();
    while(i>0)
    {
        --i;
        if(!this->mChunks[i].full())return i;
    }

    i = this->mChunks.size();
    this->mChunks.resize(i+1);
    return i;
}

template<typename T>
typename Pool<T>::Iterator& Pool<T>::Iterator::operator++()
{
    ++this->chunkIter;
    if(!this->chunkIter)
    {
        ++vecIndex;
        if(vecIndex<this->mChunksPtr->size())
        {            
            this->chunkIter = (*mChunksPtr)[vecIndex].begin();
        }
    }


    return *this;
}

template<typename T>
typename Pool<T>::Iterator Pool<T>::Iterator::operator++(int)
{
    Pool<T>::Iterator copy = *this;
    this->operator++();
    return copy;
}

template<typename T>
typename Pool<T>::CIterator& Pool<T>::CIterator::operator++()
{
    ++this->chunkIter;
    if(!this->chunkIter)
    {
        ++vecIndex;
        if(vecIndex<this->mChunksPtr->size())
        {
            this->chunkIter = (*mChunksPtr)[vecIndex].begin();
        }
    }


    return *this;
}

template<typename T>
typename Pool<T>::CIterator Pool<T>::CIterator::operator++(int)
{
    Pool<T>::CIterator copy = *this;
    this->operator++();
    return copy;
}

#endif // MEMORYPOOL_HPP
