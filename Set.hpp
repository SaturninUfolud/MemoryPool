#ifndef SET_HPP
#define SET_HPP
#include "BST_Tree.hpp"

template<typename T>
class Set
{
public:
    Set()=default;;
    Set(const Set<T>& source)=default;
    Set(Set<T>&& source)=default;

    Set<T>& operator=(Set<T>&& source)=default;
    Set<T>& operator=(const Set<T>& source)=default;
    ~Set()=default;


    using Iterator = typename BST_Tree<T>::template Iterator<false>;
    using CIterator = typename BST_Tree<T>::template Iterator<true>;

    Iterator begin()
    {
        return this->bst_.begin();
    }

    Iterator end()
    {
        return this->bst_.end();
    }

    CIterator cbegin()const
    {
        return this->bst_.cbegin();
    }

    CIterator cend()const
    {
        return this->bst_.cend();
    }

    CIterator begin()const
    {
        return this->cbegin();
    }

    CIterator end()const
    {
        return this->cend();
    }


private:
    // drzewo BST
    BST_Tree<T> bst_;

public:
    // zwraca ilość elementów
    std::size_t size() const
    {
        return bst_.size();
    }
    // dodaje element - zwraca iterator do dodanego elementu i true, lub iterator do istniejącego elementu i false, jeżeli z jakiegoś powodu nie udało się dodać/znaleźć to zwraca false i to samo co end()
    std::pair<Iterator, bool> insert(const T &value);
    // wyszukuje element o podanej wartości - jeżeli element został znaleziony to zwraca iterator do wyszukanego elementu, jeżeli nie to zwraca to samo co end()
    Iterator find(const T &value)const;
    /*{
        return bst_.find(value);
    }*/
    // usuwa element o podanej wartości - jeżeli element został usunięty to zwraca true, jeżeli elementu o podanej wartości nie udało się odnaleźć to zwraca false
    bool remove(const T& value);

    void debug1()const
    {
        bst_.debug1();
    }

    void debugGraphical()const
    {
        bst_.debugGraphical();
    }
};

template<typename T>
typename Set<T>::Iterator Set<T>::find(const T &value)const
{
    BST_Tree<T> * t1 = const_cast<BST_Tree<T> *>(&this->bst_);
    return t1->find(value);
}

template<typename T>
std::pair<typename Set<T>::Iterator, bool> Set<T>::insert(const T& value)
{
    Iterator it = bst_.find(value);
    if(it!=this->end())return std::make_pair(it,false);

    return std::make_pair(bst_.insert(value),true);
}

template<typename T>
bool Set<T>::remove(const T&value)
{
    Iterator it = bst_.find(value);
    if(it!=this->end())
    {
        bst_.remove(value);
        return true;
    }
    return false;
}

#endif // SET_HPP
