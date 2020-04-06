TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Tester.cpp \
        main.cpp

HEADERS += \
    BST_Tree.hpp \
    Map.hpp \
    MemoryPool.hpp \
    MemoryPoolChunk.hpp \
    PoolIteratorProxy.hpp \
    Set.hpp \
    Tester.h \
    chunkTest.hpp \
    mapTest.hpp \
    poolTest.hpp \
    setTest.hpp \
    treeTest.hpp
