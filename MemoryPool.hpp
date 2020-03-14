#ifndef MEMORYPOOL_HPP
#define MEMORYPOOL_HPP
#include "MemoryPoolChunk.hpp"
#include <vector>
#include <stack>
#include <type_traits>

#define _REMOVE_EMPTY_CHUNKS

template<typename T>
class Pool
{
public:
    Pool()
        :mSize(0)
    {

    }
    Pool(const Pool&pool);
    Pool(Pool&&pool);
    ~Pool()
    {
        clear();
    }

    void clear();


    Pool& operator=(const Pool&source);
    Pool& operator=(Pool&&source);

    template<bool IsConst = false>
    class Iterator
    {
        using reference = typename std::conditional< IsConst, T const &, T & >::type;
        using pointer = typename std::conditional< IsConst, T const *, T * >::type;
        using ChunksVectorPtr = typename std::conditional
            < IsConst, const std::vector<PoolChunk<T>*>*, std::vector<PoolChunk<T>*>* >::type;

        using ChunkIterator = typename PoolChunk<T>::template Iterator<IsConst>;

    public:
        Iterator()
            :vecIndex(0),mChunksPtr(nullptr),chunkIter(ChunkIterator())
        {

        }

        template<typename U=std::enable_if<IsConst>>
        Iterator(const Iterator<false>&nonConstIterator)
            :vecIndex(nonConstIterator.vecIndex)
            ,mChunksPtr(nonConstIterator.mChunksPtr)
            ,chunkIter(nonConstIterator.chunkIter)
        {

        }

        Iterator& operator++();
        Iterator operator++(int);

        Iterator& operator+=(size_t d1);
        Iterator operator+(size_t d1)const;

        reference operator*()const
        {
            return *chunkIter;
        }

        pointer operator->()const
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

        /*operator pointer()const
        {
            std::cout<<"To ptr"<<std::endl;
            return (bool)(*this)? &(this->operator*()) : nullptr;
        }*/

        void setNull()
        {
            this->mChunksPtr = nullptr;
            this->chunkIter.setNull();
        }

    private:
        friend class Pool;
        Iterator(std::size_t vecIndex, ChunksVectorPtr mChunksPtr,ChunkIterator chunkIter)
        :vecIndex(vecIndex),mChunksPtr(mChunksPtr),chunkIter(chunkIter)
        {

        }
        std::size_t vecIndex;
        ChunksVectorPtr mChunksPtr;
        ChunkIterator chunkIter;


        //typename PoolChunk<T>::Iterator chunkIter;
    };
    Iterator<false>begin()
    {
        return empty()?Iterator<false>():Iterator<false>(0,&mChunks,mChunks[0]->begin());
    }
    Iterator<false>end()
    {
        return empty()?Iterator<false>():Iterator<false>(mChunks.size(),&mChunks,mChunks[mChunks.size()-1]->end());
    }

    Iterator<true> cbegin()const
    {
        return empty()?Iterator<true>():Iterator<true>(0,&mChunks,mChunks[0]->begin());
    }
    Iterator<true> cend()const
    {
        return empty()?Iterator<true>():Iterator<true>(mChunks.size(),&mChunks,mChunks[mChunks.size()-1]->end());
    }

    Iterator<true> begin()const
    {
        return this->cbegin();
    }

    Iterator<true> end()const
    {
        return this->cend();
    }

    Iterator<false> copyIterator(const Iterator<false>&source);
    Iterator<true> copyIterator(const Iterator<true>&source)const;

    Iterator<false> insert(const T&item);
    Iterator<false> insert(T&&item);

    void remove(Iterator<false>& iter);    

    std::size_t size()const
    {
        return this->mSize;
    }

    bool empty()const
    {
        return this->mSize==0;
    }

private:
    std::vector<PoolChunk<T>*> mChunks;
    std::size_t mSize;
    std::size_t reserveChunk();
};

template<typename T>
Pool<T>::Pool(const Pool&source)
    :mSize(source.mSize)
{
    this->mChunks.reserve(source.mChunks.size());
    for(const PoolChunk<T>* chunk:source.mChunks)
    {
        this->mChunks.push_back(new PoolChunk<T>(*chunk));
    }
}

template<typename T>
Pool<T>::Pool(Pool&&source)
    :mChunks(std::move(source.mChunks)),mSize(source.mSize)
{
    source.mSize=0;
}

template<typename T>
void Pool<T>::clear()
{
    if(!this->mChunks.empty())
    {
        for(PoolChunk<T>* &chunk:mChunks)
        {
            if(chunk!=nullptr)
            {
                delete chunk;
                chunk=nullptr;
            }
        }
        this->mChunks.clear();
    }
    this->mSize = 0;
}

template <typename T>
Pool<T>& Pool<T>::operator=(const Pool&source)
{
    if(this!=&source)
    {
        clear();

        this->mSize = source.mSize;
        this->mChunks.reserve(source.mChunks.size());
        for(const PoolChunk<T>* chunk:source.mChunks)
        {
            this->mChunks.push_back(new PoolChunk<T>(*chunk));
        }
    }
    return *this;
}

template <typename T>
Pool<T>& Pool<T>::operator=(Pool&&source)
{
    if(this!=&source)
    {
        clear();

        this->mSize = source.mSize;
        this->mChunks = std::move(source.mChunks);

        source.mSize=0;
    }
    return *this;
}

template<typename T>
typename Pool<T>::template Iterator<false> Pool<T>::copyIterator(const Iterator<false>&source)
{
    if(source.vecIndex<this->mChunks.size())
    {
        return Pool<T>::Iterator<false>
            (source.vecIndex,&this->mChunks,this->mChunks[source.vecIndex]->copyIterator(source.chunkIter));
    }
    return Pool<T>::Iterator<false>();
}

template<typename T>
typename Pool<T>::template Iterator<true> Pool<T>::copyIterator(const Iterator<true>&source)const
{
    if(source.vecIndex<this->mChunks.size())
    {
        return Pool<T>::Iterator<true>
            (source.vecIndex,&this->mChunks,this->mChunks[source.vecIndex]->copyIterator(source.chunkIter));
    }
    return Pool<T>::Iterator<true>();
}

template<typename T>
typename Pool<T>::template Iterator<false> Pool<T>::insert(const T&item)
{
    std::size_t vecIndex = this->reserveChunk();
    typename PoolChunk<T>::template Iterator<false> chunkIter = this->mChunks[vecIndex]->insert(item);
    return Pool<T>::Iterator<false>(vecIndex,&this->mChunks,chunkIter);
}

template<typename T>
typename Pool<T>::template Iterator<false> Pool<T>::insert(T&&item)
{
    std::size_t vecIndex = this->reserveChunk();
    typename PoolChunk<T>::template Iterator<false> chunkIter
        = this->mChunks[vecIndex]->insert(std::move(item));
    return Pool<T>::Iterator<false>(vecIndex,&this->mChunks,chunkIter);
}



template<typename T>
void Pool<T>::remove(Iterator<false>&iter)
{
    if(empty())throw "[Pool] The pool is empty!";

    --this->mSize;

    if(!iter)throw "[Pool] Trying to remove null iterator.";
    else if(iter.mChunksPtr!=&this->mChunks)throw "[Pool] Trying to remove iterator to the other pool.";

    std::size_t vecIndex = iter.vecIndex;

    PoolChunk<T>*chunk = this->mChunks[vecIndex];
    chunk->remove(iter.chunkIter);
    iter.setNull();


#ifdef _REMOVE_EMPTY_CHUNKS
    if(chunk->empty())
    {
        delete chunk;
        std::size_t s1 = this->mChunks.size() - 1;

        if(s1 > 0 && s1 != vecIndex)
        {
            this->mChunks[vecIndex] = this->mChunks[s1];
        }
        this->mChunks.resize(s1);
    }
#endif
}

template<typename T>
std::size_t Pool<T>::reserveChunk()
{
    ++this->mSize;

    std::size_t i = this->mChunks.size();
    while(i>0)
    {
        --i;
        if(!this->mChunks[i]->full())return i;
    }

    i = this->mChunks.size();
    //this->mChunks.resize(i+1);
    this->mChunks.push_back(new PoolChunk<T>());

    return i;
}

template<typename T>
template<bool IsConst>
typename Pool<T>::template Iterator<IsConst>& Pool<T>::Iterator<IsConst>::operator++()
{
    ++this->chunkIter;
    if(!this->chunkIter)
    {
        ++vecIndex;
        if(vecIndex<this->mChunksPtr->size())
        {
            this->chunkIter = this->mChunksPtr->operator[](vecIndex)->begin();
        }
    }

    return *this;
}

template<typename T>
template<bool IsConst>
typename Pool<T>::template Iterator<IsConst> Pool<T>::Iterator<IsConst>::operator++(int)
{
    Pool<T>::Iterator<IsConst> copy = *this;
    this->operator++();
    return copy;
}

template<typename T>
template<bool IsConst>
typename Pool<T>::template Iterator<IsConst>& Pool<T>::Iterator<IsConst>::operator+=(size_t d1)
{
    for(size_t i=0;i<d1;++i)
    {
        this->operator++();
    }
    return *this;
}

template<typename T>
template<bool IsConst>
typename Pool<T>::template Iterator<IsConst> Pool<T>::Iterator<IsConst>::operator+(size_t d1)const
{
    Pool<T>::Iterator<IsConst> copy = *this;
    for(size_t i=0;i<d1;++i)
    {
        copy.operator++();
    }
    return copy;

}

#endif // MEMORYPOOL_HPP
