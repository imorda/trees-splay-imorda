#pragma once

#include <Node.h>
#include <vector>

class SplayTree
{
public:
    bool contains(int value) const;

    bool insert(int value);

    bool remove(int value);

    std::size_t size() const;

    bool empty() const;

    std::vector<int> values() const;

    ~SplayTree();

private:
    std::size_t sz = 0;
    Node * root = nullptr;

    void splay(Node * target);

    Node * split(int);

    void merge(Node *);

    Node * max() const;

    Node * min() const;

    static void rotate(Node &);

    static void values_impl_dfs(std::vector<int> &, Node *);
};
