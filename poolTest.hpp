#pragma once

#include "Tester.h"
#include "MemoryPool.hpp"

inline void poolTest()
{
    std::cout<<"A0"<<std::endl;

    Pool<Tester> pool1;
    for(const Tester&t1:pool1)
    {
        std::cout<<t1<<std::endl;
    }

    auto it1 = pool1.insert(Tester());
    std::cout<<"A1"<<std::endl;
    for(int i=0;i<12;++i)
    {
        pool1.insert(Tester());
    }

    std::cout<<"A2"<<std::endl;
    for(const Tester&t1:pool1)
    {
        std::cout<<t1<<std::endl;
    }

    std::cout<<"A3"<<std::endl;

    std::cout<<"### "<<it1->getID()<<std::endl;

    std::cout<<"it1: "<<(bool)it1<<std::endl;

    pool1.remove(it1);

    std::cout<<"it1: "<<(bool)it1<<std::endl;

    std::cout<<"A4"<<std::endl;

    for(const Tester&t1:pool1)
    {
        std::cout<<t1<<std::endl;
    }

    std::cout<<"A5"<<std::endl;

    Pool<Tester>::Iterator<false> itNull;
    std::cout<<"itNull:"<<(bool)itNull<<std::endl;
    itNull.setNull();
    std::cout<<"itNull:"<<(bool)itNull<<std::endl;

    std::cout<<"A6"<<std::endl;

    {
        const Pool<Tester> pool2 = pool1;

        for(const Tester&t:pool2)
        {
            std::cout<<t<<std::endl;
        }

        Pool<Tester> pool3 = pool1;

        pool1.insert(Tester());

        for(const Tester&t:pool3)
        {
            std::cout<<t<<std::endl;
        }

        std::cout<<"A7"<<std::endl;

        pool3 = pool2;

        for(const Tester&t:pool3)
        {
            std::cout<<t<<std::endl;
        }

        std::cout<<"A8"<<std::endl;

        pool3 = std::move(pool1);

        std::cout<<"A9"<<std::endl;

        for(const Tester&t:pool3)
        {
            std::cout<<t<<std::endl;
        }

        std::cout<<"A9a"<<std::endl;

        for(const Tester&t:pool1)
        {
            std::cout<<t<<std::endl;
        }

        std::cout<<"A10"<<std::endl;
    }

    std::cout<<"A11"<<std::endl;

}

template<typename Iterator>
void printInterator(const char*name,Iterator it)
{
    std::cout<<name<<":";
    if(it)
    {
        std::cout<<"true";
    }
    else
    {
        std::cout<<"false";
    }
    std::cout<<std::endl;
}


inline void poolTest2()
{
    std::cout<<"F1"<<std::endl;

    Pool<int>p;

    for(int i=0;i<50;++i)
    {
        p.insert(i);
    }

    auto it = p.insert(50);

    for(int i=51;i<100;++i)
    {
        p.insert(i);
    }

    std::cout<<"F2"<<std::endl;

    for(int& i :p)
    {
        i*=i;
    }

    p.remove(it);

    for(const int& i:p)
    {
        std::cout<<i<<std::endl;
    }

    std::cout<<"F3"<<std::endl;
    {
        Pool<int>::Iterator<false> iter1;

        std::cout<<"iter1: "<<(bool)iter1<<std::endl;

        iter1 = p.begin();
        iter1+=4;

        std::cout<<"F3a"<<std::endl;

        printInterator("iter1",iter1);

        std::cout<< *iter1 << std::endl;
        std::cout<<"F3b"<<std::endl;
        Pool<int> p1 = p;

        auto iter2 = p1.copyIterator(iter1);
        *iter2 = 20;

        std::cout<< "*iter1 = " <<*iter1<< std::endl;
        std::cout<< "*iter2 = " <<*iter2<< std::endl;

        std::cout<<"F3c"<<std::endl;

        Pool<int> p2 = std::move(p1);
        iter2 = p2.copyIterator(iter2);

        std::cout<< "*iter1 = " <<*iter1<< std::endl;
        std::cout<< "*iter2 = " <<*iter2<< std::endl;

        using iterator = Pool<int>::Iterator<false>;
        iterator iter3 = p2.end();

        //std::cout<<"iter3: "<<(bool)iter3<<std::endl;
        printInterator("iter3",iter3);



    }
    std::cout<<"F4"<<std::endl;
}

