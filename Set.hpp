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

private:
    // drzewo BST
    BST_Tree<T> bst_;

public:
    // zwraca ilość elementów
    std::size_t size() const
    {
        return this->bst_.size();
    }
    // zwraca true gdy zbiór jest pusty, lub false w innym przypadku
    bool empty() const
    {
        return this->bst_.empty();
    }
    // dodaje element - zwraca adres dodanego, lub istniejącego elementu
    const T* insert(const T& value);
    // wyszukuje element o podanej wartości - jeżeli element został znaleziony to zwraca wskaźnik do wyszukanego elementu, jeżeli nie to zwraca nullptr
    //const T* find(const T& value) const;
    // j.w.
    const T* find(const T& value)const
    {
        return bst_.find(value);
    }
    // usuwa element o podanej wartości - jeżeli element został usunięty to zwraca true, jeżeli elementu o podanej wartości nie udało się odnaleźć to zwraca false;
    bool remove(const T& value);
    /*{
        bst_.remove(value);
    }*/

    void debug1()const
    {
        bst_.debug1();
    }
};

template<typename T>
const T* Set<T>::insert(const T&value)
{
    const T* ptr = bst_.find(value);
    if(ptr!=nullptr)return ptr;
    else return bst_.insert(value);
}

template<typename T>
bool Set<T>::remove(const T&value)
{
    const T* ptr = bst_.find(value);
    if(ptr!=nullptr)
    {
        bst_.remove(value);
        return true;
    }
    return false;
}

#endif // SET_HPP
