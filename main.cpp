#include <iostream>
#include <bitset>
#include "MemoryPool.hpp"
#include "Tester.h"
//#include "chunkTest.hpp"
//#include "poolTest.hpp"
#include "treeTest.hpp"
#include "setTest.hpp"

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
