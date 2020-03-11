#pragma once

#include "MemoryPoolChunk.hpp"
#include "Tester.h"
#include <vector>

inline void chunkTest()
{
    {
        std::cout<<"P0"<<std::endl;
        PoolChunk<Tester> chunk;
        std::cout<<"P1"<<std::endl;

        auto it1 = chunk.insert(Tester());

        std::cout<<"P2"<<std::endl;

        chunk.insert(Tester());
        Tester * t2 = new Tester();
        chunk.insert(*t2);
        delete t2;

        std::cout<<"P3"<<std::endl;

        chunk.remove(it1);
        std::cout<<"P4"<<std::endl;

        chunk.debug();

        chunk.insert(Tester());

        std::cout<<"P5"<<std::endl;
        for(Tester& t: chunk)
        {
            std::cout<<t<<std::endl;
        }

        std::cout<<"P6"<<std::endl;
        {
            const PoolChunk<Tester> cChunk = chunk;

            for(const Tester& t:cChunk)
            {
                std::cout<<t<<std::endl;
            }
        }

        std::cout<<"P6"<<std::endl<<std::endl;

    }
}


inline void chunkTest2()
{

    PoolChunk<Tester> s;




    auto t1 = s.insert(Tester());
    Tester l1;

    for(int i=0;i<10;++i)
    {
        s.insert(Tester());
    }

    auto t3 = s.insert(Tester());


    for(int i=0;i<10;++i)
    {
        s.insert(Tester());
    }


    std::vector<PoolChunk<Tester>> vs, vs2;
    vs.resize(1000);

    std::cout<<"D3"<<std::endl;

    for(auto& e : vs)
    {
        e = s;
        e.insert(Tester());
    }
    vs2.resize(1000);
    for(std::size_t i = 0u; i < vs.size(); ++i) { vs2[i] = std::move(vs[i]); }
    for(auto& e : vs2) { e.remove(t1); }
    vs = vs2;
    vs2.clear();


    std::cout<<"D4"<<std::endl;

    const auto s2 = std::move(vs.front());
    std::cout<<"D5"<<std::endl;

    //if(auto f = s2.find(t2)) std::cout << "Found: " << *f << std::endl;
    auto s3 = s2;
    s3.debug();
    s = s3;

    std::cout<<"D6"<<std::endl;
}

