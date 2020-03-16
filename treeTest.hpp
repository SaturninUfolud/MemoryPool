#pragma once

#include "BST_Tree.hpp"
#include "Tester.h"
#include <string>

#include "PoolIteratorProxy.hpp"

/*
class BST_Node
{
public:
    int item;
    Pool<BST_Node>::Iterator<false> lChild;
    Pool<BST_Node>::Iterator<false> rChild;
};

class BST_Node1
{
public:
    int item;
    PoolIteratorProxy<BST_Node,false> lChild;
    PoolIteratorProxy<BST_Node,true>  rChild;
};
*/

constexpr std::size_t _ITERATOR_SIZE = sizeof(Pool<void*>::Iterator<false>);

inline void treeTest()
{
    std::cout<<_ITERATOR_SIZE<<std::endl;

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

    /*tree.toRightVine();

    tree.debugGraphical();*/

    std::cout<<"B4"<<std::endl;
    {
        BST_Tree<Tester> tree1 = tree;

        std::cout<<"B4a"<<std::endl;
        tree1.debugGraphical();

        std::cout<<"B4b"<<std::endl;


        BST_Tree<Tester> tree2 = std::move(tree1);
        std::cout<<"B4c"<<std::endl;

        tree2.debugGraphical();

    }

    std::cout<<"B5"<<std::endl;

    for(auto it = tree.begin();it != tree.end();++it)
    {
        std::cout<<*it<<", ";
    }

    for(const Tester&t : tree)
    {
        std::cout<<t<<std::endl;
    }

    std::cout<<std::endl;
    std::cout<<"B6"<<std::endl;
}

inline void treeTest2()
{
    std::cout<<"T0"<<std::endl;

    BST_Tree<double> tree1;

    double da1[15] = {-1,5,3,0,4,7,8,13,-12,7.1,1,11,2,17,7.2};
    double sum = 0;


    for(size_t i =0 ;i<15;++i)
    {
        tree1.insert(da1[i]);
        sum+=da1[i];
    }


    for(auto it = tree1.begin();it != tree1.end();++it)
    {
        std::cout<<it<<", ";
    }

    std::cout<<std::endl;

    std::cout<<"T1"<<std::endl;

    tree1.debugGraphical();

    std::cout<<"T2"<<std::endl;

    const double*ptr = tree1.find(13);
    if(ptr!=nullptr)
    {
        std::cout<<*ptr<<std::endl;
    }
    else
    {
        std::cout<<"nullptr"<<std::endl;
    }

    std::cout<<"T3"<<std::endl;

    tree1.dsw();

    std::cout<<"T4"<<std::endl;
    tree1.debugGraphical();

    std::cout<<"T5"<<std::endl;
}
