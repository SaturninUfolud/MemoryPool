#pragma once

#include "BST_Tree.hpp"
#include "Tester.h"

inline void treeTest()
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

inline void treeTest2()
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
