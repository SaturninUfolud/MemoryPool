#ifndef SETTEST_HPP
#define SETTEST_HPP

#include "Set.hpp"

using namespace std;

template<typename Iter>
void printDebugInfo(const std::pair<Iter, bool>& insert_info)
{
    std::cout << (insert_info.second ? "Dodano " + std::to_string(*insert_info.first) : "Nie dodano") << std::endl;
}

template<typename Iter>
void printDebugInfo(const Iter& find_info)
{
    std::cout << (find_info ? "Znaleziono " + std::to_string(*find_info) : "Nie znaleziono") << std::endl;
}

void printDebugInfo(bool removed)
{
    std::cout << (removed ? "Usunięto" : "Nie usunięto") << std::endl;
}

template<typename T>
void printSetSizeAndElems(const Set<T>& s)
{
    std::cout << "Size: " << s.size() << std::endl;
    for(const auto& e : s) std::cout << e << ", ";
    std::cout << std::endl;
}
inline void setTestIter()
{
    std::vector<int> v{1, 2, 3, 4};
    for(auto it = v.begin(); it != v.end(); ++it) { std::cout << *it << ", "; }
    std::cout << std::endl;
    for(const auto& e : v) { std::cout << e << ", "; }
    std::cout << std::endl;

    Set<int> s;

    {
        printDebugInfo(s.insert(8));
        printDebugInfo(s.insert(4));
        printDebugInfo(s.insert(10));
        printDebugInfo(s.insert(10));
        printDebugInfo(s.insert(2));
        printDebugInfo(s.insert(5));
        printDebugInfo(s.insert(9));
        printDebugInfo(s.insert(12));
        printDebugInfo(s.insert(12));
        printDebugInfo(s.insert(1));
        printDebugInfo(s.insert(14));
        printDebugInfo(s.insert(7));
        std::cout << std::endl;

        std::cout << "Size: " << s.size() << std::endl;
        for(auto it = s.begin(); it != s.end(); ++it) { std::cout << *it << ", "; }
        std::cout << std::endl;
        for(const auto& e : s) { std::cout << e << ", "; }
        std::cout << std::endl;

        auto it = s.begin();
        std::cout << *++it << std::endl;
        std::cout << std::endl;

        auto it2 = s.find(10);
        printDebugInfo(it2);
        // *it2 = 2;
        printDebugInfo(s.find(5));
        printDebugInfo(s.remove(4));
        printDebugInfo(s.find(5));
        printDebugInfo(s.remove(12));
        printDebugInfo(s.remove(14));
        printDebugInfo(s.insert(0));
        printDebugInfo(s.insert(16));
        printDebugInfo(s.remove(8));
        std::cout << std::endl;
        printSetSizeAndElems(s);
        std::vector<Set<int>> vs, vs2;
        vs.resize(1000);
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
        const auto s2 = std::move(vs.front());
        printDebugInfo(s2.find(5));
        auto s3 = s2;
        printSetSizeAndElems(s3);
        printDebugInfo(s3.remove(2));
        printDebugInfo(s3.remove(2));
        s = s3;
    }
    printSetSizeAndElems(s);

}


inline void setTest()
{
    Set<int> s;
    {
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
        const auto s2 = std::move(vs.front());
        if(auto f = s2.find(5)) std::cout << "Found: " << *f << std::endl;
        auto s3 = s2;
        printSetSizeAndElems(s3);
        s = s3;
    }
    printSetSizeAndElems(s);
}

#endif // SETTEST_HPP
