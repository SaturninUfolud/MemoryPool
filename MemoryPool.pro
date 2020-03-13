TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Tester.cpp \
        main.cpp

HEADERS += \
    BST_Tree.hpp \
    BST_TreeOld.hpp \
    MemoryPool.hpp \
    MemoryPoolChunk.hpp \
    Set.hpp \
    Tester.h \
    chunkTest.hpp \
    poolTest.hpp \
    setTest.hpp \
    treeTest.hpp
