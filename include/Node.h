#pragma once

class Node
{
public:
    Node(int _value)
        : value(_value)
    {
    }

    Node * left = nullptr;
    Node * right = nullptr;
    Node * parent = nullptr;

    int getValue() const
    {
        return value;
    }

    ~Node()
    {
        delete left;
        delete right;
    }

private:
    const int value;
};