#include <Node.h>

Node *Node::lower_max_val(const int value, Node *cur_node) {
    if (cur_node == nullptr || cur_node->getValue() == value) {
        return cur_node;
    }
    if (cur_node->getValue() > value) {
        return lower_max_val(value, cur_node->left);
    }

    Node *right = lower_max_val(value, cur_node->right);
    if (right == nullptr) {
        return cur_node;
    }

    return right;
}

void Node::update_children_parents(Node &parent) {
    if (parent.left != nullptr) {
        parent.left->parent = &parent;
    }
    if (parent.right != nullptr) {
        parent.right->parent = &parent;
    }
}
