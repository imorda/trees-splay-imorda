#pragma once

class Node {
public:
    Node(int value)
            : m_value(value) {
    }

    Node *left = nullptr;
    Node *right = nullptr;
    Node *parent = nullptr;

    int getValue() const {
        return m_value;
    }

    ~Node() {
        delete left;
        delete right;
    }

    static void update_children_parents(Node &);

    static Node *lower_max_val(int value, Node *); // Finds max value in tree in range [min(), value]

private:
    const int m_value;
};
