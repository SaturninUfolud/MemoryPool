#ifndef MEMORYPOOLCHUNK_HPP
#define MEMORYPOOLCHUNK_HPP

#include <bitset>
#include <iostream>

#define N 64

template<typename T>
class PoolChunk
{
public:
    PoolChunk() = default;

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
        return *(reinterpret_cast<T*>(mBuffer1)+index);
    }

    const T& getItem(std::size_t index)const
    {
        return *(reinterpret_cast<const T*>(mBuffer1)+index);
    }

    void debug()const;

    class Iterator
    {
    public:
        Iterator()
            :mChunk(nullptr),index(N)
        {

        }

        Iterator& operator++();
        Iterator operator++(int);
        T& operator*()const
        {
            return this->mChunk->getItem(index);
        }

        T* operator->()const
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

    private:
        Iterator(PoolChunk<T>*mChunk, size_t index)
            :mChunk(mChunk),index(index)
        {

        }

        friend class PoolChunk;
        PoolChunk<T> * mChunk;
        size_t index;
    };

    Iterator begin();
    Iterator end()
    {
        return Iterator(this,N);
    }

    Iterator insert(const T& item);
    Iterator insert(T&& item);

    void remove(Iterator& iter);

    class CIterator
    {
    public:
        CIterator()
            :mChunk(nullptr),index(N)
        {

        }

        CIterator& operator++();
        CIterator operator++(int);
        const T& operator*()const
        {
            return this->mChunk->getItem(index);
        }

        const T* operator->()const
        {
            return &(this->operator*());
        }

        bool operator==(const CIterator&it2)const
        {
            return it2.index==this->index&&it2.mChunk==this->mChunk;
        }

        bool operator!=(const CIterator&it2)const
        {
            return !(it2==*this);
        }

        operator bool()const
        {
            return index<N;
        }

    private:
        CIterator(const PoolChunk<T>*mChunk, size_t index)
            :mChunk(mChunk),index(index)
        {

        }

        friend class PoolChunk;
        const PoolChunk<T> * mChunk;
        size_t index;
    };

    CIterator begin()const;
    CIterator end()const
    {
        return CIterator(this,N);
    }


private:
    void mCopyHelper(const PoolChunk&source);
    void mCopyHelper(PoolChunk&&source)noexcept;

    void putAtIndex(const T&item,int index)
    {
        new (mBuffer1 + index*sizeof (T)) T(item);
    }
    void putAtIndex(T&&item,int index)
    {
        new (mBuffer1 + index*sizeof (T)) T(std::move(item));
    }


    size_t reserveSpace();
    char mBuffer1[N * sizeof (T)];
    std::bitset<N> mBitset;
};



template<typename T>
void PoolChunk<T>::mCopyHelper(const PoolChunk& source)
{
    if(!source.empty())
    {
        const T*ptr = reinterpret_cast<const T*>(source.mBuffer1);

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
void PoolChunk<T>::mCopyHelper(PoolChunk&& source)noexcept
{
    if(!source.empty())
    {
        T*ptr = reinterpret_cast<T*>(source.mBuffer1);

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
        T * ptr = reinterpret_cast<T*>(mBuffer1);
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
typename PoolChunk<T>::Iterator PoolChunk<T>::insert(const T&item)
{
    size_t i = this->reserveSpace();
    this->putAtIndex(item,i);
    return Iterator(this,i);
}

template <typename T>
typename PoolChunk<T>::Iterator PoolChunk<T>::insert(T&&item)
{
    size_t i = this->reserveSpace();
    this->putAtIndex(std::move(item),i);
    return Iterator(this,i);
}

template <typename T>
void PoolChunk<T>::debug()const
{
    std::cout<<this->mBitset<<std::endl;
    const T* ptr = reinterpret_cast<const T*const>(this->mBuffer1);

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
void PoolChunk<T>::remove(Iterator&iter)
{
    size_t index = iter.index;

    if(!this->mBitset[index])throw "[PoolChunk] Trying to remove nullptr";
    (reinterpret_cast<T*>(this->mBuffer1)+index) -> ~T();
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
typename PoolChunk<T>::Iterator& PoolChunk<T>::Iterator::operator++()
{
    do
    {
        ++index;

    }while(index<N && !this->mChunk->mBitset[index]);

    return *this;
}

template<typename T>
typename PoolChunk<T>::Iterator PoolChunk<T>::Iterator::operator++(int)
{
    auto copy = *this;
    this->operator++();
    return copy;
}

template<typename T>
typename PoolChunk<T>::Iterator PoolChunk<T>::begin()
{
    Iterator it1 = Iterator(this,0);
    while(!this->mBitset[it1.index] && it1.index<N)
    {
        ++it1.index;
    }
    return it1;
}

template<typename T>
typename PoolChunk<T>::CIterator& PoolChunk<T>::CIterator::operator++()
{
    do
    {
        ++index;

    }while(index<N && !this->mChunk->mBitset[index]);

    return *this;
}

template<typename T>
typename PoolChunk<T>::CIterator PoolChunk<T>::CIterator::operator++(int)
{
    auto copy = *this;
    this->operator++();
    return copy;
}

template<typename T>
typename PoolChunk<T>::CIterator PoolChunk<T>::begin()const
{
    CIterator it1 = CIterator(this,0);
    while(!this->mBitset[it1.index] && it1.index<N)
    {
        ++it1.index;
    }
    return it1;
}

#endif // MEMORYPOOLCHUNK_HPP
