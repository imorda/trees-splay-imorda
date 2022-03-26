#include <SplayTree.h>


bool SplayTree::contains(int value) const
{
    SplayTree * self = const_cast<SplayTree*>(this);

    Node * target = lower_max_val_impl_dfs(value, root);
    if(target == nullptr){
        self->splay(min());
        return false;
    }

    self->splay(target);

    return target->getValue() == value;
}

bool SplayTree::insert(int value)
{
    Node * right = split(value);

    splay(max());
    if(root != nullptr && root->getValue() == value){
        merge(right);
        return false;
    }

    Node* left = root;

    root = new Node(value);

    root->left = left;
    root->right = right;
    update_children_parents(root);
    sz++;
    return true;
}

bool SplayTree::remove(int value)
{
    Node * target = lower_max_val_impl_dfs(value, root);
    if (target == nullptr || target->getValue() != value) {
        return false;
    }
    splay(target);

    Node * left = root->left;
    Node * right = root->right;

    root->left = nullptr;
    root->right = nullptr;

    delete root;
    sz--;

    root = left;
    if (left != nullptr) {
        left->parent = nullptr;
    }
    if(right != nullptr){
        right->parent = nullptr;
    }
    merge(right);

    return true;
}

std::size_t SplayTree::size() const
{
    return sz;
}

bool SplayTree::empty() const
{
    return size() == 0;
}

std::vector<int> SplayTree::values() const
{
    std::vector<int> ans = std::vector<int>();
    ans.reserve(size());
    values_impl_dfs(ans, root);
    return ans;
}

void SplayTree::splay(Node * target)
{
    if(target == nullptr){
        return;
    }

    while(target->parent != nullptr){
        Node * parent = target->parent;
        Node * grandparent = parent->parent;
        if(target == parent->left){
            if(grandparent == nullptr){
                rotate_right(parent); //invalidates parent, grandparent
            } else if (parent == grandparent->left){
                rotate_right(grandparent); //invalidates parent, grandparent
                rotate_right(target->parent);
            } else {
                rotate_right(parent); //invalidates parent, grandparent
                rotate_left(target->parent);
            }
        } else {
            if(grandparent == nullptr){
                rotate_left(parent); //invalidates parent, grandparent
            } else if (parent == grandparent->right){
                rotate_left(grandparent); //invalidates parent, grandparent
                rotate_left(target->parent);
            } else {
                rotate_left(parent); //invalidates parent, grandparent
                rotate_right(target->parent);
            }
        }
    }
    root = target;
}

Node * SplayTree::split(int value) //extracts and returns a bigger subtree than value
{
    Node * found = lower_max_val_impl_dfs(value, root);
    if(found == nullptr){
        Node * temp = root;
        root = nullptr;
        return temp;
    }

    splay(found);
    if(root == nullptr){
        return nullptr;
    }

    Node * ans;
    if(root->getValue() <= value) {
        ans = root->right;
        root->right = nullptr;
    } else {
        ans = root;
        root = ans -> left;
        ans->left = nullptr;
        if (root != nullptr){
            root->parent = nullptr;
        }
    }

    if(ans != nullptr){
        ans->parent = nullptr;
    }

    return ans;
}

void SplayTree::merge(Node * other) //other must be bigger than the whole SplayTree
{
    if(root == nullptr){
        root = other;
        return;
    }
    splay(max()); // After this operation max element is root and since it's max, there is no right child
    root->right = other;
    update_children_parents(root);
}

Node * SplayTree::max() const
{
    Node * current = root;
    if(current == nullptr){
        return nullptr;
    }

    while(current->right != nullptr){
        current = current->right;
    }
    return current;
}

Node * SplayTree::min() const
{
    Node * current = root;
    if(current == nullptr){
        return nullptr;
    }

    while(current->left != nullptr){
        current = current->left;
    }
    return current;
}

SplayTree::~SplayTree()
{
    delete root;
}

void update_children_parents(Node * parent)
{
    if(parent == nullptr){
        return;
    }
    if(parent->left != nullptr){
        parent->left->parent = parent;
    }
    if(parent->right != nullptr){
        parent->right->parent = parent;
    }
}

void values_impl_dfs(std::vector<int>& answer, Node * cur_node)
{
    if(cur_node == nullptr){
        return;
    }

    values_impl_dfs(answer, cur_node->left);
    answer.push_back(cur_node->getValue());
    values_impl_dfs(answer, cur_node->right);
}

Node * lower_max_val_impl_dfs(const int value, Node * cur_node)
{
    if(cur_node == nullptr || cur_node->getValue() == value){
        return cur_node;
    }
    if(cur_node->getValue() > value){
        return lower_max_val_impl_dfs(value, cur_node->left);
    }

    Node * right = lower_max_val_impl_dfs(value, cur_node->right);
    if(right == nullptr){
        return cur_node;
    }

    if(right->getValue() > cur_node->getValue()){
        return right;
    }
    return cur_node;
}

void rotate_left(Node * target)
{
    Node * parent = target->parent;
    Node * right_child = target->right;

    if(right_child == nullptr){
        return;
    }

    if(parent != nullptr){
        if(parent->left == target){
            parent->left = right_child;
        } else {
            parent->right = right_child;
        }
    }

    Node * temp = right_child->left;
    right_child->left = target;
    target->right = temp;

    target->parent = right_child;
    right_child->parent = parent;

    if(target->right != nullptr){
        target->right->parent = target;
    }
}

void rotate_right(Node * target)
{
    Node * parent = target->parent;
    Node * left_child = target->left;

    if(left_child == nullptr){
        return;
    }

    if(parent != nullptr){
        if(parent->left == target){
            parent->left = left_child;
        } else {
            parent->right = left_child;
        }
    }

    Node * temp = left_child->right;
    left_child->right = target;
    target->left = temp;

    target->parent = left_child;
    left_child->parent = parent;

    if(target->left != nullptr){
        target->left->parent = target;
    }
}