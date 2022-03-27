#include <SplayTree.h>

bool SplayTree::contains(int value) const {
    SplayTree *self = const_cast<SplayTree *>(this);

    Node *target = Node::lower_max_val(value, root);
    if (target == nullptr) {
        self->splay(min());
        return false;
    }

    self->splay(target);

    return target->getValue() == value;
}

bool SplayTree::insert(int value) {
    Node *right = split(value);

    splay(max());
    if (root != nullptr && root->getValue() == value) {
        merge(right);
        return false;
    }

    Node *left = root;

    root = new Node(value);

    root->left = left;
    root->right = right;
    Node::update_children_parents(*root);
    sz++;
    return true;
}

bool SplayTree::remove(int value) {
    Node *target = Node::lower_max_val(value, root);
    if (target == nullptr || target->getValue() != value) {
        return false;
    }
    splay(target);

    Node *left = root->left;
    Node *right = root->right;

    root->left = nullptr;
    root->right = nullptr;

    delete root;
    sz--;

    root = left;
    if (left != nullptr) {
        left->parent = nullptr;
    }
    if (right != nullptr) {
        right->parent = nullptr;
    }
    merge(right);

    return true;
}

std::size_t SplayTree::size() const {
    return sz;
}

bool SplayTree::empty() const {
    return size() == 0;
}

std::vector<int> SplayTree::values() const {
    std::vector<int> ans;
    ans.reserve(size());
    values_impl_dfs(ans, root);
    return ans;
}

void SplayTree::splay(Node *target) {
    if (target == nullptr) {
        return;
    }

    while (target->parent != nullptr) {
        Node *parent = target->parent;
        Node *grandparent = parent->parent;
        if (grandparent == nullptr) {
            rotate(*target);
        } else if (parent == grandparent->left) {
            rotate(*parent);
            rotate(*target);
        } else {
            rotate(*target);
            rotate(*target);
        }
    }
    root = target;
}

Node *SplayTree::split(int value) // extracts and returns a bigger subtree than value
{
    Node *found = Node::lower_max_val(value, root);
    if (found == nullptr) {
        return std::exchange(root, nullptr);
    }

    splay(found);

    Node *ans;
    ans = root->right;
    root->right = nullptr;

    if (ans != nullptr) {
        ans->parent = nullptr;
    }

    return ans;
}

void SplayTree::merge(Node *other) // other must be bigger than the whole SplayTree
{
    if (root == nullptr) {
        root = other;
        return;
    }
    splay(max()); // After this operation max element is root and since it's max, there is no right child
    root->right = other;
    Node::update_children_parents(*root);
}

Node *SplayTree::max() const {
    Node *current = root;
    if (current == nullptr) {
        return nullptr;
    }

    while (current->right != nullptr) {
        current = current->right;
    }
    return current;
}

Node *SplayTree::min() const {
    Node *current = root;
    if (current == nullptr) {
        return nullptr;
    }

    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

SplayTree::~SplayTree() {
    delete root;
}


void SplayTree::values_impl_dfs(std::vector<int> &answer, Node *cur_node) {
    if (cur_node == nullptr) {
        return;
    }

    values_impl_dfs(answer, cur_node->left);
    answer.push_back(cur_node->getValue());
    values_impl_dfs(answer, cur_node->right);
}

void SplayTree::rotate(Node &target) {
    Node *parent = target.parent;
    if (parent == nullptr) {
        return;
    }

    Node *grandparent = parent->parent;

    if (grandparent != nullptr) {
        if (grandparent->left == parent) {
            grandparent->left = &target;
        } else {
            grandparent->right = &target;
        }
    }
    target.parent = grandparent;

    if (parent->right == &target) {
        parent->right = target.left;
        target.left = parent;
    } else {
        parent->left = target.right;
        target.right = parent;
    }

    Node::update_children_parents(*parent);
    Node::update_children_parents(target);
}
