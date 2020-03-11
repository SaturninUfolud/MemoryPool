#pragma once

#include "Tester.h"
#include "MemoryPool.hpp"

inline void poolTest()
{

    Pool<Tester> pool1;
    auto it1 = pool1.insert(Tester());
    std::cout<<"A2"<<std::endl;
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

    it1 = Pool<Tester>::Iterator::nullIter;

    std::cout<<"it1: "<<(bool)it1<<std::endl;

    std::cout<<"A4"<<std::endl;

    for(const Tester&t1:pool1)
    {
        std::cout<<t1<<std::endl;
    }

    std::cout<<"A5"<<std::endl;

    Pool<Tester>::Iterator itNull;
    std::cout<<"itNull:"<<(bool)itNull<<std::endl;
    itNull.setNull();
    std::cout<<"itNull:"<<(bool)itNull<<std::endl;

    std::cout<<"A6"<<std::endl;
}

void poolTest2()
{
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

    for(int& i :p)
    {
        i*=i;
    }



    p.remove(it);

    for(const int& i:p)
    {
        std::cout<<i<<std::endl;
    }
}
