#ifndef BST_TREE_HPP
#define BST_TREE_HPP

#include "MemoryPool.hpp"
#include "PoolIteratorProxy.hpp"
#include <type_traits>

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

    /*~BST_Tree();*/
    BST_Tree& operator=(const BST_Tree&source);
    BST_Tree& operator=(BST_Tree&&source);

    class Node
    {

    public:
        using NodePtr = typename Pool<typename BST_Tree::Node>::template Iterator<false>;

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
            //return *reinterpret_cast<NodePtr*>(lChild);
        }
        NodePtr& right()
        {
            return rChild;
            //return *reinterpret_cast<NodePtr*>(rChild);
        }
    private:
        /*char lChild[_POOL_ITERATOR_SIZE];
        char rChild[_POOL_ITERATOR_SIZE];*/

        PoolIteratorProxy<Node, false> lChild;
        PoolIteratorProxy<Node, false> rChild;
    };
    using NodePtr = typename Node::NodePtr;

    bool empty()const
    {
        return !(bool)root;
    }

    T&top()
    {
        return root->item;
    }

    const T&findMax()const;
    const T&findMin()const;

    T* insert(const T&item)
    {
        return mInsert(mNodePool.insert(item));
    }

    T* insert(T&&item)
    {
        return mInsert(mNodePool.insert(std::move(item)));
    }

    const T* find(const T&item)const;


    void remove(const T&item);

    //TODO********
    void toRightList();
    void dsw();
    //**********

    void debug1()const;
    void debugGraphical()const
    {
        printBT("",this->root,false);
    }

    std::size_t size()const
    {
        return this->mNodePool.size();
    }

private:
    NodePtr root;
    T* mInsert(NodePtr ptr);
    NodePtr mRemove(NodePtr node);
    NodePtr mCopyTreeRecursive(const NodePtr& oldRoot);

    Pool<typename BST_Tree<T>::Node> mNodePool;

    static void printBT(const std::string& prefix, const NodePtr& node, bool isLeft);

    void mCompress(NodePtr& node,int n);

    static void mRotateLeftHelper(NodePtr& nodeX);
    static void mRotateRightHelper(NodePtr& nodeX);
};


template<typename T>
BST_Tree<T>::BST_Tree(const BST_Tree&source)
    :mNodePool(source.mNodePool)
{
    this->root = mCopyTreeRecursive(source.root);
}

template<typename T>
BST_Tree<T>::BST_Tree(BST_Tree&&source)
    :mNodePool(std::move(source.mNodePool))
{
    this->root = mCopyTreeRecursive(source.root);
    source.root.setNull();
}

template <typename T>
BST_Tree<T>& BST_Tree<T>::operator=(const BST_Tree&source)
{
    if(this!=&source)
    {
        this->mNodePool = source.mNodePool;
        this->root = mCopyTreeRecursive(source.root);
    }
    return *this;
}

template<typename T>
BST_Tree<T>& BST_Tree<T>::operator=(BST_Tree&&source)
{
    if(this!=&source)
    {
        this->mNodePool = std::move(source.mNodePool);
        this->root = mCopyTreeRecursive(source.root);
        source.root.setNull();
    }
    return *this;
}


template<typename T>
typename BST_Tree<T>::NodePtr BST_Tree<T>::mCopyTreeRecursive(const NodePtr& oldRoot)
{

    NodePtr newRoot;
    if(oldRoot)
    {
        //std::cout<<"F1-in"<<std::endl;

        newRoot = this->mNodePool.copyIterator(oldRoot);

        //std::cout<<"F1-in-end"<<std::endl;

        if(newRoot->left())
        {
            newRoot->left() = mCopyTreeRecursive(oldRoot->left());
        }

        if(newRoot->right())
        {
            newRoot->right() = mCopyTreeRecursive(oldRoot->right());
        }



    }
    return newRoot;
}

template<typename T>
const T& BST_Tree<T>::findMax()const
{
    if(!root)throw "Empty tree";
    NodePtr node = root;
    while(node->left())
    {
        node = node->left();
    }

    return node->item;
}

template<typename T>
const T& BST_Tree<T>::findMin()const
{
    if(!root)throw "Empty tree";
    NodePtr node = root;
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
    NodePtr node = this->root;

    while(node)
    {
        if(node->item==item)
        {
            if(node == this->root)
            {
                this->root = this->mRemove(node);
            }
            else if(node == parent->left())
            {
                parent->left() = this->mRemove(node);
            }
            else
            {
                parent->right() = this->mRemove(node);
            }

            break;
        }

        parent = node;

        if(item<node->item)
        {
            node = node->right();
        }
        else
        {
            node = node->left();
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

inline int log2func(int n)
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
void BST_Tree<T>::toRightList()
{
    if(empty())return;

    while(root->left())
    {
        mRotateLeftHelper(root);
    }


    NodePtr parent = this->root;
    for(NodePtr n1 = root->right(); n1; n1=n1->right())
    {
        while(n1->left())
        {
            //n1 = mRotateRightHelper(n1);//mRotateRight(n1);
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
    this->toRightList();

    int size = 0;

    std::cout<<"DSW A4"<<std::endl;

    for(NodePtr node = root; node; node=node->right())
    {
        size++;
    }

    std::cout<<"DSW A5"<<std::endl;

    int leaves = size + 1 - log2func(size+1);
    mCompress(root,leaves);
    size = size -1;
    while(size > 1)
    {
        size = size/2;
        mCompress(root,size);
    }

    std::cout<<"DSW A6"<<std::endl;//*/
}

template<typename T>
void BST_Tree<T>::mCompress(NodePtr& node,int n)
{
    NodePtr scanner = node;
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
    //return nodeY;
}

template <typename T>
void BST_Tree<T>::mRotateRightHelper(NodePtr& nodeX)
{
    //std::cout<<"R-R1"<<std::endl;

    if(nodeX && nodeX->right())
    {
        NodePtr nodeY = nodeX;
        std::cout<<"R-R1.1"<<std::endl;

        nodeY = nodeX->right();
        nodeX->right() = nodeY->left();
        nodeY->left() = nodeX;

        std::cout<<"R-R1.2"<<std::endl;
        nodeX = nodeY;
    }
    else
    {
        std::cout<<"Kurwa"<<std::endl;
    }
    //std::cout<<"R-R2"<<std::endl;
    //return nodeY;
}

template<typename T>
T* BST_Tree<T>::mInsert(NodePtr newNode)
{
    //std::cout<<"mInsert begin"<<std::endl;
    if(empty())
    {
        //std::cout<<"mInsert empty"<<std::endl;
        this->root=newNode;
    }
    else
    {
        //std::cout<<"mInsert not empty"<<std::endl;
        NodePtr iter = root;
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
    return &newNode->item;
}

template<typename T>
const T* BST_Tree<T>::find(const T&item)const
{
    NodePtr node =root;

    while(node)
    {
        if(item==node->item)
        {
            return &node->item;
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

    return nullptr;
}

template<typename T>
typename BST_Tree<T>::NodePtr BST_Tree<T>::mRemove(NodePtr node)
{
    if(!node)return node;

    NodePtr newRoot;
    //Node * newRoot = nullptr;

    if(!node->left())
    {
        newRoot = node->right();
    }
    else if(!node->right())
    {
        newRoot = node->left();
    }
    else
    {
        newRoot = node->left();

        NodePtr n1 = node->left();

        while(n1->right())
        {
            n1 = n1->right();
        }

        n1->right() = node->right();
    }

    this->mNodePool.remove(node);

    return newRoot;
}


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



#endif // BST_TREE_HPP
