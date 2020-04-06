#ifndef MAP_H
#define MAP_H


#pragma once

#include <cstdint>
#include <utility>
#include <iterator>
#include "BST_Tree.hpp"

// Uwaga! Kod powinien być odporny na błędy i każda z metod jeżeli zachodzi niebezpieczeństwo wywołania z niepoprawnymi parametrami powinna zgłaszać odpowiednie wyjątki!

// klasa reprezentująca słownik (map) oparty na drzewie BST/AVL
// K - typ kluczy
// V - typ wartości
template <typename K, typename V>
class Map
{
public:
    class KeyValuePair
    {
    public:
        KeyValuePair(const K& key)
            :key(key)
        {

        }

        KeyValuePair(const K& key, const V& value)
            :key(key),value(value)
        {

        }
        KeyValuePair(const std::pair<K,V>&pairIn)
            :key(pairIn.first),value(pairIn.second)
        {

        }

        KeyValuePair(const KeyValuePair&source)
            :key(source.key), value(source.value)
        {

        }

        KeyValuePair(KeyValuePair&&source)
            :key(std::move(source.key)), value(std::move(source.value))
        {

        }

        KeyValuePair& operator=(const KeyValuePair&source)
        {
            this->key = source.key;
            this->value = source.value;
            return *this;
        }

        KeyValuePair& operator=(KeyValuePair&&source)
        {
            this->key = std::move(source.key);
            this->value = std::move(source.value);
            return *this;
        }


        bool operator==(const KeyValuePair&source)const
        {
            return this->key == source.key;
        }

        bool operator>=(const KeyValuePair&source)const
        {
            return this->key >= source.key;
        }

        bool operator<=(const KeyValuePair&source)const
        {
            return this->key <= source.key;
        }

        bool operator>(const KeyValuePair&source)const
        {
            return this->key > source.key;
        }

        bool operator<(const KeyValuePair&source)const
        {
            return this->key < source.key;
        }

        K key;
        mutable V value;
    };


    class Iterator
    {
    private:
        friend class Map;
        using TreeIterator = typename BST_Tree<KeyValuePair>::Iterator;

        TreeIterator treeIter;

        Iterator(const TreeIterator& treeIter)
            :treeIter(treeIter)
        {

        }
    public:
        Iterator() = default;
        bool operator==(const Iterator &it) const
        {
            return this->treeIter==it.treeIter;
        }
        bool operator!=(const Iterator &it) const
        {
            return this->treeIter!=it.treeIter;
        }
        Iterator& operator++()
        {
            this->treeIter.operator++();
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator copy = *this;
            this->operator++();
            return copy;
        }
        V& operator*() const
        {
            return treeIter->value;
        }

        V* operator->() const
        {
            return &this->operator*();
        }

        operator bool() const
        {
            return (bool)this->treeIter;
        }
    };

    class ConstIterator
    {
    private:
        friend class Map;
        using TreeIterator = typename BST_Tree<KeyValuePair>::Iterator;
        TreeIterator treeIter;

        ConstIterator(const TreeIterator& treeIter)
            :treeIter(treeIter)
        {

        }

        ConstIterator(const Iterator& nonConstIter)
            :treeIter(nonConstIter.treeIter)
        {

        }

    public:
        bool operator==(const ConstIterator &it) const
        {
            return this->treeIter==it.treeIter;
        }
        bool operator!=(const ConstIterator &it) const
        {
            return this->treeIter!=it.treeIter;
        }
        ConstIterator& operator++()
        {
            this->treeIter.operator++();
            return *this;
        }
        ConstIterator operator++(int)
        {
            Iterator copy = *this;
            this->operator++();
            return copy;
        }
        const V& operator*() const
        {
            return treeIter->value;
        }
        const V* operator->() const
        {
            return &this->operator*();
        }
        operator bool() const
        {
            return (bool)this->treeIter;
        }
    };

    // ...


private:
    BST_Tree<KeyValuePair> mTree;

public:
    Map() = default;
    Map(const Map<K, V> &source) = default;
    Map<K, V>& operator=(const Map<K, V> &source) = default;
    Map(Map<K, V> &&source) = default;
    Map<K, V>& operator=(Map<K, V> &&source) = default;
    ~Map() = default;

    // zwraca ilość elementów
    std::size_t size() const
    {
        return mTree.size();
    }
    // dodaje klucz i wartość - zwraca "Iterator" do dodanej wartości i true, lub "Iterator" do istniejącej wartości i false, jeżeli z jakiegoś powodu nie udało się dodać/znaleźć to zwraca false i to samo co end()
    std::pair<Iterator, bool> insert(const std::pair<K, V> &key_value);
    // wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca "ConstIterator" do znalezionej wartości, jeżeli nie to zwraca to samo co end()
    ConstIterator find(const K &key) const
    {
        return ConstIterator(this->mTree.find(key));
    }
    // wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca "Iterator" do znalezionej wartości, jeżeli nie to zwraca to samo co end()
    Iterator find(const K &key)
    {
        return Iterator(this->mTree.find(key));
    }
    // wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca referencję do znalezionej (stałej) wartości, jeżeli nie to dodaje nowy element o podanym kluczu i domyślnej wartości V() i zwraca referencję do wartości
    const V& operator[](const K &key) const;
    // wyszukuje element o podanym kluczu - jeżeli element został znaleziony to zwraca referencję do znalezionej wartości, jeżeli nie to dodaje nowy element o podanym kluczu i domyślnej wartości V() i zwraca referencję do wartości
    V& operator[](const K &key);
    // usuwa element o podanej wartości - jeżeli element został usunięty to zwraca "Iterator" na kolejny element, jeżeli elementu o podanej wartości nie udało się odnaleźć to zwraca to samo co "end()"
    Iterator remove(const K &key);
    // usuwa wszystkie elementy
    void clear()
    {
        mTree.clear();
    }

    // zwraca "ConstIterator" na pierwszy element
    ConstIterator begin() const
    {
        return ConstIterator(this->mTree.begin());
    }
    // zwraca "Iterator" na pierwszy element
    Iterator begin()
    {
        return Iterator(this->mTree.begin());
    }
    // zwraca "ConstIterator" "za ostatni" element
    ConstIterator end() const
    {
        return ConstIterator(this->mTree.end());
    }
    // zwraca "Iterator" "za ostatni" element
    Iterator end()
    {
        return Iterator(this->mTree.end());
    }
};

template <typename K, typename V>
std::pair<typename Map<K,V>::Iterator, bool> Map<K,V>::insert(const std::pair<K,V>& key_value)
{
    auto it = this->mTree.find(key_value);
    if(it)return std::make_pair(Iterator(it),false);
    it = this->mTree.insert(key_value);
    if(it)return std::make_pair(Iterator(it),true);
}


template <typename K, typename V>
V& Map<K,V>::operator[](const K &key)
{
    KeyValuePair newPair = key;
    auto it = this->mTree.find(newPair);
    if(!it)
    {
        it = this->mTree.insert(newPair);
    }
    return it->value;
}

template <typename K, typename V>
const V& Map<K,V>::operator[](const K &key)const
{
    auto it = this->mTree.find(key);
    if(!it)
    {
        throw "[Map] element not found";
    }
    return it->value;
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::remove(const K &key)
{
    auto iter = this->mTree.find(key);
    auto iterCopy = iter;
    if(iter)
    {
        ++iterCopy;
        this->mTree.remove(iter);
    }
    return iterCopy;
}

#endif // MAP_H
