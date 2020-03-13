#pragma once

#include "MemoryPoolChunk.hpp"
#include "Tester.h"
#include <vector>

inline void chunkTest()
{
    {
        std::cout<<"P0"<<std::endl;
        PoolChunk<Tester> chunk;

        for(const Tester&t:chunk)
        {
            std::cout<<t<<std::endl;
        }

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

        //auto it = chunk.begin();
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

            std::cout<<"P6a"<<std::endl;
            PoolChunk<Tester> chunk1 = chunk;
            chunk1.debug();

            std::cout<<"P6b"<<std::endl;

            PoolChunk<Tester> chunk2 = std::move(chunk);
            chunk2.debug();

            std::cout<<"P6c"<<std::endl;
            chunk.debug();

            std::cout<<"P6d"<<std::endl;
            chunk = std::move(chunk1);

        }

        std::cout<<"P7"<<std::endl;
        {
            PoolChunk<Tester>::Iterator<false> it2 = chunk.begin();
            PoolChunk<Tester>::Iterator<false> it3 = it2;
            PoolChunk<Tester>::Iterator<true> it4 = it3;
            PoolChunk<Tester>::Iterator<true> it5 = it4;
            std::cout<<*it5<<std::endl;
        }
        std::cout<<"P8"<<std::endl;
    }
}


