#include <iostream>
#include <bitset>
#include "MemoryPool.hpp"
#include "Tester.h"

//#include "poolTest.hpp"
#include "treeTest.hpp"
//#include "setTest.hpp"
//#include "mapTest.hpp"

int main()
{
    std::cout<<"Hello world"<<std::endl;
    try
    {
        //chunkTest();
        //poolTest();
        //poolTest2();
        //treeTest();
        treeTest2();
        //setTest();
        //setTestIter();
        //mapTest();
    }
    catch (const char*e)
    {
        std::cout<<e<<std::endl;
    }
    catch(...)
    {
        std::cout<<"Unknown exception"<<std::endl;
    }

    std::cout<<"\n#######\nPowinno być 0, a jest: "<<Tester::getCounter()<<std::endl;

    return 0;
}
