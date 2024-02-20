#pragma once

#include "AVLTree.h"

template<typename T>
class Set {
private:
    AVLTree<T> tree;

public:
    Set() : tree() {}

    void insert(const T &value) { tree.insert(value); }
    void remove(const T &value) { tree.remove(value); }
    bool contain(const T &value) { return tree.contain(value); }
    bool empty() { return tree.empty(); }
    T& minimum() { return tree.minimum(); }
};