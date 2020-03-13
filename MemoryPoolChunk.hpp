#ifndef MEMORYPOOLCHUNK_HPP
#define MEMORYPOOLCHUNK_HPP

#include <bitset>
#include <iostream>
#include <type_traits>
//#include <iterator>


#define N 64


template<typename T>
class PoolChunk
{
public:
    PoolChunk()=default;

    PoolChunk(const PoolChunk&source)
        :mBitset(source.mBitset)
    {
        this->mCopyHelper(source);
    }

    PoolChunk(PoolChunk&&source)noexcept
        :mBitset(source.mBitset)
    {
        this->mCopyHelper(std::move(source));
    }

    ~PoolChunk()
    {
        this->clear();
    }

    void clear();

    PoolChunk&operator=(PoolChunk&&source);
    PoolChunk&operator=(const PoolChunk&source);

    bool full()const
    {
        return this->mBitset.all();
    }

    bool empty()const
    {
        return this->mBitset.none();
    }

    T& getItem(std::size_t index)
    {
        return *(reinterpret_cast<T*>(mBuffer)+index);
    }

    const T& getItem(std::size_t index)const
    {
        return *(reinterpret_cast<const T*>(mBuffer)+index);
    }

    void debug()const;

    template<bool IsConst=false>
    class Iterator
    {
        using reference = typename std::conditional< IsConst, T const &, T & >::type;
        using pointer = typename std::conditional< IsConst, T const *, T * >::type;
        using pointer_chunk = typename std::conditional< IsConst, const PoolChunk<T>*, PoolChunk<T>* >::type;

    public:
        Iterator()
            :mChunk(nullptr),index(N)
        {

        }

        template<typename U = std::enable_if<IsConst>>
        Iterator(const Iterator<false>&nonConstIt)
            :mChunk(nonConstIt.mChunk),index(nonConstIt.index)
        {
            //std::cout<<"Conversion"<<std::endl;
        }


        Iterator& operator++();
        Iterator operator++(int);

        reference operator*()const
        {
            return this->mChunk->getItem(this->index);
        }
        pointer operator->()const
        {
            return &(this->operator*());
        }


        bool operator==(const Iterator&it2)const
        {
            return it2.index==this->index&&it2.mChunk==this->mChunk;
        }

        bool operator!=(const Iterator&it2)const
        {
            return !(it2==*this);
        }

        operator bool()const
        {
            return index<N;
        }

        void setNull()
        {
            this->index=N;
        }

    private:
        Iterator(pointer_chunk mChunk, size_t index)
            :mChunk(mChunk),index(index)
        {

        }

        friend class PoolChunk;

        pointer_chunk mChunk;

        size_t index;
    };

    Iterator<false> copyIterator(const Iterator<false>&source)
    {
        return Iterator<false>(this,source.index);
    }

    Iterator<true> copyIterator(const Iterator<true>&source)const
    {
        return Iterator<true>(this,source.index);
    }

    Iterator<false> begin();
    Iterator<false> end()
    {
        return Iterator<false>(this,N);
    }

    Iterator<false> insert(const T& item);
    Iterator<false> insert(T&& item);

    void remove(Iterator<false>& iter);

    Iterator<true> cbegin()const;
    Iterator<true> cend()const
    {
         return Iterator<true>(this,N);
    }


    Iterator<true> begin()const
    {
        return this->cbegin();
    }
    Iterator<true> end()const
    {
        return this->cend();
    }


private:
    void mCopyHelper(const PoolChunk&source);
    void mCopyHelper(PoolChunk&&source);

    void putAtIndex(const T&item,int index)
    {
        new (mBuffer + index*sizeof (T)) T(item);
    }
    void putAtIndex(T&&item,int index)
    {
        new (mBuffer + index*sizeof (T)) T(std::move(item));
    }


    size_t reserveSpace();
    char mBuffer[sizeof (T)* N];

    std::bitset<N> mBitset;
};
template<typename T>
void PoolChunk<T>::mCopyHelper(const PoolChunk& source)
{
    if(!source.empty())
    {
        const T*ptr = reinterpret_cast<const T*>(source.mBuffer);

        for(std::size_t i=0;i<N;++i)
        {
            if(mBitset[i])
            {
                this->putAtIndex(*ptr,i);
            }
            ptr++;
        }
    }
}

template<typename T>
void PoolChunk<T>::mCopyHelper(PoolChunk&& source)
{
    //std::cout<<"move chunk"<<std::endl;

    if(!source.empty())
    {
        T*ptr = reinterpret_cast<T*>(source.mBuffer);

        for(std::size_t i=0;i<N;++i)
        {
            if(mBitset[i])
            {
                this->putAtIndex(std::move(*ptr),i);
            }
            ptr++;
        }
    }
}

template<typename T>
void PoolChunk<T>::clear()
{
    if(!this->empty())
    {
        T * ptr = reinterpret_cast<T*>(mBuffer);
        for(std::size_t i =0; i<N;++i)
        {
            if(mBitset[i])ptr->~T();
            ++ptr;
        }
        mBitset.reset();
    }
}

template<typename T>
PoolChunk<T>& PoolChunk<T>::operator=(const PoolChunk&source)
{
    if(this!=&source)
    {
        this->clear();

        this->mBitset = source.mBitset;
        this->mCopyHelper(source);
    }
    return *this;
}


template<typename T>
PoolChunk<T>& PoolChunk<T>::operator=(PoolChunk&&source)
{
    if(this!=&source)
    {
        this->clear();
        this->mBitset = source.mBitset;
        this->mCopyHelper(std::move(source));
    }

    return *this;
}



template <typename T>
typename PoolChunk<T>::template Iterator<false> PoolChunk<T>::insert(const T&item)
{
    size_t i = this->reserveSpace();
    this->putAtIndex(item,i);
    return Iterator<false>(this,i);
}

template <typename T>
typename PoolChunk<T>::template Iterator<false> PoolChunk<T>::insert(T&&item)
{
    size_t i = this->reserveSpace();
    this->putAtIndex(std::move(item),i);
    return Iterator<false>(this,i);
}

template <typename T>
void PoolChunk<T>::debug()const
{
    std::cout<<this->mBitset<<std::endl;
    const T* ptr = reinterpret_cast<const T*const>(this->mBuffer);

    for(std::size_t i =0; i<N; ++i)
    {
        if(this->mBitset[i])
        {
            std::cout<<*ptr<<std::endl;
        }
        ++ptr;
    }
}

template<typename T>
void PoolChunk<T>::remove(Iterator<false>&iter)
{
    size_t index = iter.index;

    if(!this->mBitset[index])throw "[PoolChunk] Trying to remove nullptr";
    (reinterpret_cast<T*>(this->mBuffer)+index) -> ~T();
    this->mBitset[index] = false;
}

template <typename T>
std::size_t PoolChunk<T>::reserveSpace()
{
    for(std::size_t i = 0 ; i<N; ++i)
    {
        if(!this->mBitset[i])
        {
            this->mBitset[i] = true;
            return i;
        }
    }
    throw "[PoolChunk] The chunk is full";
}

template<typename T>
template<bool IsConst>
typename PoolChunk<T>::template Iterator<IsConst>& PoolChunk<T>::Iterator<IsConst>::operator++()
{
    do
    {
        ++index;

    }while(index<N && !this->mChunk->mBitset[index]);

    return *this;
}

template<typename T>
template<bool IsConst>
typename PoolChunk<T>::template Iterator<IsConst> PoolChunk<T>::Iterator<IsConst>::operator++(int)
{
    auto copy = *this;
    this->operator++();
    return copy;
}

template<typename T>
typename PoolChunk<T>::template Iterator<false> PoolChunk<T>::begin()
{
    Iterator<false> it1 = Iterator<false>(this,0);
    while(!this->mBitset[it1.index] && it1.index<N)
    {
        ++it1.index;
    }
    return it1;
}

template<typename T>
typename PoolChunk<T>::template Iterator<true> PoolChunk<T>::cbegin()const
{
    Iterator<true> it1 = Iterator<true>(this,0);
    while(!this->mBitset[it1.index] && it1.index<N)
    {
        ++it1.index;
    }
    return it1;
}

#endif // MEMORYPOOLCHUNK_HPP
