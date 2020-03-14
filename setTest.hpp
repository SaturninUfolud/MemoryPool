#pragma once

#include "Set.hpp"
#include "Tester.h"


template<typename T>
void printSetSizeAndElems(const Set<T>& s)
{
    std::cout << "Size: " << s.size() << std::endl;
    //s.inorder([](const T& e) { std::cout << e << ", "; });
    s.debug1();

    std::cout << std::endl;
}

inline void setTest()
{
    Set<int> s;
    {
        //std::cout<<"D1"<<std::endl;

        s.insert(8);
        s.insert(4);
        s.insert(10);
        s.insert(10);
        s.insert(2);
        s.insert(5);
        s.insert(9);
        s.insert(12);
        s.insert(12);
        s.insert(1);
        s.insert(14);
        s.insert(7);

        //std::cout<<"D2"<<std::endl;

        if(auto f = s.find(5)) std::cout << "Found: " << *f << std::endl;
        s.remove(4);
        if(auto f = s.find(5)) std::cout << "Found: " << *f << std::endl;
        s.remove(12);
        s.remove(14);
        s.insert(0);
        s.insert(16);
        s.remove(8);
        printSetSizeAndElems(s);
        std::vector<Set<int>> vs, vs2;
        vs.resize(1000);

        //std::cout<<"D3"<<std::endl;

        for(auto& e : vs)
        {
            e = s;
            e.insert(13);
        }
        vs2.resize(1000);
        for(std::size_t i = 0u; i < vs.size(); ++i) { vs2[i] = std::move(vs[i]); }
        for(auto& e : vs2) { e.remove(10); }
        vs = vs2;
        vs2.clear();


        //std::cout<<"D4"<<std::endl;

        const auto s2 = std::move(vs.front());
        //std::cout<<"D5"<<std::endl;

        if(auto f = s2.find(5)) std::cout << "Found: " << *f << std::endl;
        auto s3 = s2;
        printSetSizeAndElems(s3);
        s = s3;

        //std::cout<<"D6"<<std::endl;  //*/
    }
    printSetSizeAndElems(s);

}
