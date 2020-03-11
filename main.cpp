#include <iostream>
#include <bitset>
#include "MemoryPool.hpp"
#include "BST_Tree.hpp"
#include "Set.hpp"

#include <cstdlib>
#include <ctime>
#include "Tester.h"

void chunkTest()
{
    {
        std::cout<<"P0"<<std::endl;
        PoolChunk<Tester> chunk;
        std::cout<<"P1"<<std::endl;

        //std::size_t i1 = 0;

        /*{
            Tester t1;
            i1 =  chunk.put(t1);
        }*/

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

        std::cout<<"P6"<<std::endl;

        Tester t;
    }


}

void poolTest()
{


    Pool<Tester> pool1;

    //std::cout<<"A1"<<std::endl;
    //Tester*t1 = new Tester();
    auto it1 = pool1.insert(Tester());

    //delete t1;

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

void treeTest()
{
    BST_Tree<Tester> tree;

    std::cout<<tree.empty()<<std::endl;
    std::cout<<"B1"<<std::endl;

    Tester t1;
    Tester t2;

    tree.insert(t2);

    for(int i=0;i<5;++i)
    {
        tree.insert(Tester());
    }
    std::cout<<"B2"<<std::endl;

    tree.insert(t1);
    std::cout<<"B3"<<std::endl;
    tree.debugGraphical();

    tree.toRightList();

    tree.debugGraphical();

    std::cout<<"B4"<<std::endl;
}



void treeTest2()
{
    BST_Tree<double> tree1;

    double da1[15] = {-1,5,3,0,4,7,8,13,-12,7.1,1,11,2,17,7.2};
    double sum = 0;

    for(size_t i =0 ;i<15;++i)
    {
        tree1.insert(da1[i]);
        sum+=da1[i];
    }

    tree1.debugGraphical();
}

template<typename T>
void printSetSizeAndElems(const Set<T>& s)
{
    std::cout << "Size: " << s.size() << std::endl;
    //s.inorder([](const T& e) { std::cout << e << ", "; });
    s.debug1();

    std::cout << std::endl;
}

void setTest()
{
    Set<int> s;
    {
        std::cout<<"D1"<<std::endl;

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

        std::cout<<"D2"<<std::endl;

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

        std::cout<<"D3"<<std::endl;

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


        std::cout<<"D4"<<std::endl;

        //const auto s2 = std::move(vs.front());

        const auto s2 = vs.front();

        //const Set<int> s2 = std::move(vs.front());

        std::cout<<"D5"<<std::endl;

        if(auto f = s2.find(5)) std::cout << "Found: " << *f << std::endl;
        auto s3 = s2;
        printSetSizeAndElems(s3);
        s = s3;

        std::cout<<"D6"<<std::endl;  //*/
    }
    printSetSizeAndElems(s);

}


int main()
{
    std::cout<<"Hello world"<<std::endl;
    try
    {
        //chunkTest();
        //poolTest();
        //poolTest2();
        //treeTest();
        //treeTest2();
        setTest();
    }
    catch (const char*e)
    {
        std::cout<<e<<std::endl;
    }
    catch(...)
    {
        std::cout<<"Dupa blada"<<std::endl;
    }

    std::cout<<"\n#######\nPowinno być 0, a jest: "<<Tester::getCounter()<<std::endl;

    return 0;
}
