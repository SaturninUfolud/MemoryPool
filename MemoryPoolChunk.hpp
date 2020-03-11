#ifndef MEMORYPOOLCHUNK_HPP
#define MEMORYPOOLCHUNK_HPP

#include <bitset>
#include <iostream>

#define N 64

template<typename T>
class PoolChunk
{
public:
    PoolChunk();
    PoolChunk(const PoolChunk&source);

    PoolChunk(PoolChunk&&source)noexcept;
    ~PoolChunk();
    /*{
        this->clear();
    }*/



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

    T& getItem(std::size_t index)const
    {
        return *(reinterpret_cast<T*>(mBuffer)+index);
    }

    //void remove(size_t index);

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
    /*{
        //std::cout<<"Begin\n";
        return Iterator(this,0);
    }*/

    Iterator end()
    {
        //std::cout<<"End\n";
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
    /*{
        return CIterator(this,0);
    }*/

    CIterator end()const
    {
        return CIterator(this,N);
    }


private:
    void clear();

    void putAtIndex(const T&item,int index)
    {
        new (mBuffer + index*sizeof (T)) T(item);
    }
    void putAtIndex(T&&item,int index)
    {
        new (mBuffer + index*sizeof (T)) T(std::move(item));
    }


    size_t reserveSpace();
    unsigned char * mBuffer;
    std::bitset<N> mBitset;
};

template<typename T>
PoolChunk<T>::PoolChunk()
{
    mBuffer = new unsigned char[N*sizeof (T)];
}

template<typename T>
PoolChunk<T>::PoolChunk(const PoolChunk&source)
{


    mBuffer = new unsigned char[N*sizeof (T)];

    if(!source.empty())
    {
        T*ptr = reinterpret_cast<T*>(source.mBuffer);

        for(std::size_t i=0;i<N;++i)
        {
            if(source.mBitset[i])
            {
                this->mBitset[i]=true;
                this->putAtIndex(*ptr,i);
            }
            ptr++;
        }
    }
    //std::cout<<"EFGH\n";
}
template<typename T>
PoolChunk<T>::~PoolChunk()
{
    this->clear();

    if(mBuffer!=nullptr)
    {
        delete [] mBuffer;
        mBuffer=nullptr;
    }
}

template<typename T>
PoolChunk<T>::PoolChunk(PoolChunk&&source)noexcept
    :mBuffer(source.mBuffer),mBitset(std::move(source.mBitset))
{
    source.mBuffer=nullptr;
    source.mBitset.reset();

    //std::cout<<"ABCD\n";
}

template<typename T>
PoolChunk<T>& PoolChunk<T>::operator=(const PoolChunk&source)
{
    if(this!=&source)
    {
        this->clear();
        this->mBitset = source.mBitset;

        //mBuffer = new unsigned char[N*sizeof (T)];


        if(!source.empty())
        {
            T*ptr = reinterpret_cast<T*>(source.mBuffer);

            for(std::size_t i=0;i<N;++i)
            {
                if(source.mBitset[i])
                {
                    this->mBitset[i]=true;
                    this->putAtIndex(*ptr,i);
                }
                ptr++;
            }
        }

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
        this->mBuffer = source.mBuffer;

        source.mBuffer=nullptr;
        source.mBitset.reset();
    }

    return *this;
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

template <typename T>
typename PoolChunk<T>::Iterator PoolChunk<T>::insert(const T&item)
{
    size_t i = this->reserveSpace();
    this->putAtIndex(item,i);
    //new (mBuffer + i*sizeof (T)) T(item);
    return Iterator(this,i);
}

template <typename T>
typename PoolChunk<T>::Iterator PoolChunk<T>::insert(T&&item)
{
    size_t i = this->reserveSpace();
    this->putAtIndex(std::move(item),i);
    //new (mBuffer + i*sizeof (T)) T(std::move(item));
    return Iterator(this,i);
}

template <typename T>
void PoolChunk<T>::debug()const
{
    std::cout<<this->mBitset<<std::endl;
    T* ptr = reinterpret_cast<T*>(this->mBuffer);

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

    if(!this->mBitset[index])throw "Trying to remove nullptr";
    (reinterpret_cast<T*>(this->mBuffer)+index) -> ~T();
    this->mBitset[index] = false;
    ++iter;
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
    throw "The pool chunk is full";
}

template<typename T>
typename PoolChunk<T>::Iterator& PoolChunk<T>::Iterator::operator++()
{
    //std::cout<<"++op"<<std::endl;
    do
    {
        ++index;

    }while(index<N && !this->mChunk->mBitset[index]);

    return *this;
}

template<typename T>
typename PoolChunk<T>::Iterator PoolChunk<T>::Iterator::operator++(int)
{
    //std::cout<<"op++"<<std::endl;
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
