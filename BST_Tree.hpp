#ifndef BST_TREE_HPP
#define BST_TREE_HPP

#include "MemoryPool.hpp"
#include "PoolIteratorProxy.hpp"
#include <type_traits>
#include <functional>
#include <queue>
//constexpr std::size_t _POOL_ITERATOR_SIZE = sizeof (Pool<void*>::Iterator<false>);
template<typename T>
class BST_Tree
{

public:
    BST_Tree()
    {

    }

    BST_Tree(const BST_Tree&source);
    BST_Tree(BST_Tree&&source);

    BST_Tree& operator=(const BST_Tree&source);
    BST_Tree& operator=(BST_Tree&&source);

    class Node
    {

    public:
        //using NodePtr = typename PoolIteratorProxy<Node, false>::PoolPtr;
        using NodePtr = typename Pool<typename BST_Tree<T>::Node>::template Iterator<false>;

        Node(const T&item)
            :item(item)
        {
        }
        Node(T&&item)
            :item(std::move(item))
        {
        }

        T item;
        NodePtr& left()
        {
            return lChild;
        }
        NodePtr& right()
        {
            return rChild;
        }
    private:
        PoolIteratorProxy<Node, false> lChild;
        PoolIteratorProxy<Node, false> rChild;
    };
    using NodePtr = typename Node::NodePtr;



    const T&top()
    {
        return mRoot->item;
    }

    const T&findMax()const;
    const T&findMin()const;

    void remove(const T&item);

    /**
     * DSW algorithm
     * https://en.wikipedia.org/wiki/Day%E2%80%93Stout%E2%80%93Warren_algorithm
     */
    void dsw();
    void toRightVine();


    //**********

    void debug1()const;
    void debugGraphical()const
    {
        printBT("",this->mRoot,false);
    }

    std::size_t size()const
    {
        return this->mNodePool.size();
    }

    bool empty()const
    {
        return !(bool)this->mRoot;//this->mNodePool.empty();
    }

    class Iterator
    {
    private:
        using PoolIter = typename Pool<typename BST_Tree<T>::Node>::template Iterator<true>;

        using reference = T const &;
        using pointer = T const *;
        friend class BST_Tree;

        Iterator(const NodePtr& npt)
            :poolIter(npt)
        {

        }

    public:        
        Iterator(const PoolIter& poolIter)
            :poolIter(poolIter)
        {

        }
        reference operator*()const
        {
            //reference r = poolIter->item;
            return poolIter->item;
        }
        pointer operator->()const
        {
            return &(this->operator*());
        }

        bool operator==(const Iterator&it2)const
        {
            return this->poolIter==it2.poolIter;
        }

        bool operator!=(const Iterator&it2)const
        {
            return this->poolIter!=it2.poolIter;
        }

        operator bool()const
        {
            return (bool)poolIter;
        }

        Iterator& operator++()
        {
            ++poolIter;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator copy = *this;
            ++poolIter;
            return copy;
        }

    private:
        PoolIter poolIter;
    };

    Iterator begin()const
    {
        return Iterator(this->mNodePool.cbegin());
    }

    Iterator end()const
    {
        return Iterator(this->mNodePool.cend());
    }

    Iterator insert(const T&item)
    {
        return mInsert(mNodePool.insert(item));
    }

    Iterator insert(T&&item)
    {
        return mInsert(mNodePool.insert(std::move(item)));
    }

    Iterator find(const T&item)const;

    void clear()
    {
        mRoot.setNull();
        this->mNodePool.clear();
    }

    void remove(Iterator iter)
    {
        remove(*iter);
    }

    void inorder(const std::function<void(const T& item)>& func)const
    {
        mInorderRecursive(func,this->mRoot);
    }
private:
    NodePtr mRoot;
    Iterator mInsert(NodePtr ptr);
    NodePtr mRemove(NodePtr node);
    NodePtr mCopyTreeRecursive(const NodePtr& oldmRoot);

    Pool<typename BST_Tree<T>::Node> mNodePool;

    static void printBT(const std::string& prefix, const NodePtr& node, bool isLeft);

    void mCompress(NodePtr& mRoot,int n);
    static void mRotateLeftHelper(NodePtr& nodeX);

    static void mInorderRecursive(const std::function<void(const T& item)>& func,const NodePtr& ptr);
};


template<typename T>
BST_Tree<T>::BST_Tree(const BST_Tree&source)
    :mNodePool(source.mNodePool)
{
    this->mRoot = mCopyTreeRecursive(source.mRoot);
}

template<typename T>
BST_Tree<T>::BST_Tree(BST_Tree&&source)
    :mNodePool(std::move(source.mNodePool))
{
    this->mRoot = mCopyTreeRecursive(source.mRoot);
    source.mRoot.setNull();
}

template <typename T>
BST_Tree<T>& BST_Tree<T>::operator=(const BST_Tree&source)
{
    if(this!=&source)
    {
        this->mNodePool = source.mNodePool;
        this->mRoot = mCopyTreeRecursive(source.mRoot);
    }
    return *this;
}

template<typename T>
BST_Tree<T>& BST_Tree<T>::operator=(BST_Tree&&source)
{
    if(this!=&source)
    {
        this->mNodePool = std::move(source.mNodePool);
        this->mRoot = mCopyTreeRecursive(source.mRoot);
        source.mRoot.setNull();
    }
    return *this;
}


template<typename T>
typename BST_Tree<T>::NodePtr BST_Tree<T>::mCopyTreeRecursive(const NodePtr& oldmRoot)
{

    NodePtr newmRoot;
    if(oldmRoot)
    {
        //std::cout<<"F1-in"<<std::endl;

        newmRoot = this->mNodePool.copyIterator(oldmRoot);

        //std::cout<<"F1-in-end"<<std::endl;

        if(newmRoot->left())
        {
            newmRoot->left() = mCopyTreeRecursive(oldmRoot->left());
        }

        if(newmRoot->right())
        {
            newmRoot->right() = mCopyTreeRecursive(oldmRoot->right());
        }



    }
    return newmRoot;
}

template<typename T>
const T& BST_Tree<T>::findMax()const
{
    if(!mRoot)throw "Empty tree";
    NodePtr node = mRoot;
    while(node->left())
    {
        node = node->left();
    }

    return node->item;
}

template<typename T>
const T& BST_Tree<T>::findMin()const
{
    if(!mRoot)throw "Empty tree";
    NodePtr node = mRoot;
    while(node->right())
    {
        node = node->right();
    }

    return node->item;
}


template<typename T>
void BST_Tree<T>::remove(const T&item)
{
    NodePtr parent;
    NodePtr node = this->mRoot;

    while(node)
    {
        if(node->item==item)
        {
            if(node == this->mRoot)
            {
                this->mRoot = this->mRemove(node);
            }
            else if(node == parent->left())
            {
                parent->left() = this->mRemove(node);
            }
            else
            {
                parent->right() = this->mRemove(node);
            }

            this->mNodePool.remove(node);

            break;
        }

        parent = node;

        if(item<node->item)
        {
            node = node->left();
        }
        else
        {
            node = node->right();
        }
    }
}

template<typename T>
void BST_Tree<T>::debug1()const
{
    for(const Node&node:this->mNodePool)
    {
        std::cout<<node.item<<", ";
    }
    std::cout<<std::endl;
}


template<typename T>
typename BST_Tree<T>::Iterator BST_Tree<T>::mInsert(NodePtr newNode)
{
    //std::cout<<"mInsert begin"<<std::endl;
    if(empty())
    {
        //std::cout<<"mInsert empty"<<std::endl;
        this->mRoot=newNode;
    }
    else
    {
        //std::cout<<"mInsert not empty"<<std::endl;
        NodePtr iter = mRoot;
        while(true)
        {            
            if(newNode->item>iter->item)
            {
                if(iter->right())
                {
                    iter = iter->right();
                }
                else
                {
                    iter->right()=newNode;
                    break;
                }
            }
            else
            {
                if(iter->left())
                {
                    iter = iter->left();
                }
                else
                {
                    iter->left()=newNode;
                    break;
                }
            }
        }
    }

    //std::cout<<"mInsert end"<<std::endl;
    //return &newNode->item;

    return newNode;
}

template<typename T>
typename BST_Tree<T>::Iterator BST_Tree<T>::find(const T&item)const
{
    NodePtr node =mRoot;

    while(node)
    {
        if(item==node->item)
        {
            return node;//&node->item;
        }
        else if(item<node->item)
        {
            node = node->left();
        }
        else
        {
            node = node->right();
        }
    }

    return this->end();
    //return nullptr;
}

template<typename T>
typename BST_Tree<T>::NodePtr BST_Tree<T>::mRemove(NodePtr node)
{
    if(!node)return node;

    NodePtr newmRoot;
    //Node * newmRoot = nullptr;

    if(!node->left())
    {
        newmRoot = node->right();
    }
    else if(!node->right())
    {
        newmRoot = node->left();
    }
    else
    {
        newmRoot = node->left();

        NodePtr n1 = node->left();

        while(n1->right())
        {
            n1 = n1->right();
        }

        n1->right() = node->right();
    }

    return newmRoot;
}
/**
 *https://stackoverflow.com/questions/4965335/how-to-print-binary-tree-diagram
 */

template<typename T>
void BST_Tree<T>::printBT(const std::string& prefix, const NodePtr& node, bool isLeft)
{
    if(node)
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
        std::cout << node->item << std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->left(), true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->right(), false);
    }
}

/**
 * DSW algorithm
 * https://en.wikipedia.org/wiki/Day%E2%80%93Stout%E2%80%93Warren_algorithm
 */

inline int _log2func(int n)
{
    int r=0;
    while (n>=2)
    {
        n/=2;
        r++;
    }
    return 1<<r;
}

template<typename T>
void BST_Tree<T>::toRightVine()
{
    if(empty())return;

    while(mRoot->left())
    {
        mRotateLeftHelper(mRoot);
    }


    NodePtr parent = this->mRoot;
    for(NodePtr n1 = mRoot->right(); n1; n1=n1->right())
    {
        while(n1->left())
        {
            mRotateLeftHelper(n1);
        }
        parent->right() = n1;
        parent = n1;
    }
}

template<typename T>
void BST_Tree<T>::dsw()
{
    if(empty())return;
    this->toRightVine();

    int size = 0;

    for(NodePtr node = mRoot; node; node=node->right())
    {
        size++;
    }

    int leaves = size + 1 - _log2func(size+1);
    mCompress(mRoot,leaves);
    size = size -1;
    while(size > 1)
    {
        size = size/2;
        mCompress(mRoot,size);
    }
}

template<typename T>
void BST_Tree<T>::mCompress(NodePtr& mRoot,int n)
{
    NodePtr scanner = mRoot;
    for(int i=0;i<n;++i)
    {
        NodePtr child = scanner->right();

        scanner->right() = child->right();

        scanner = scanner->right();
        child->right() = scanner->left();
        scanner->left() = child;
    }
}

template <typename T>
void BST_Tree<T>::mRotateLeftHelper(NodePtr& nodeX)
{
    if(nodeX && nodeX->left())
    {
        NodePtr nodeY = nodeX;

        nodeY = nodeX->left();
        nodeX->left() = nodeY->right();
        nodeY->right() = nodeX;

        nodeX = nodeY;
    }
}

template <typename T>
void BST_Tree<T>::mInorderRecursive(const std::function<void(const T&item)>& func, const NodePtr&ptr)
{
    if(ptr)
    {
        if(ptr->left())mInorderRecursive(func,ptr->left());
        func(ptr->item);
        if(ptr->right())mInorderRecursive(func,ptr->right());
    }
}

#endif // BST_TREE_HPP
