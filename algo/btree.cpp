#include <iostream>
#include <sstream>

#include <queue>
#include <vector>

#include <utility>
#include <cassert>

#define NIL -1

struct Node {
    int value;
    Node* left = nullptr;
    Node* right = nullptr;   
    Node(int x): value(x) {} 
};

using Nodes = std::vector<Node*>;
using Keys = std::vector<int>;

//applies function to each node level by level
template<typename UnaryFunction>
void bfs_foreach(Node* root, UnaryFunction f) {
    if (!root)
        return;
    std::vector<Node*> siblings{root};
    while (!siblings.empty()) {
        std::vector<Node*> tmp;
        for (auto& node: siblings) {
            if (node->left)
                tmp.push_back(node->left);
            if (node->right)
                tmp.push_back(node->right);
            f(node);
        }
        siblings = std::move(tmp);
    }
}

//No recursion b-tree destroy
//Assignes nullptr to root because root may be a child node
//of another tree
void destroy(Node*& root) {
    bfs_foreach(root, [] (Node* x) { delete x; } );
    root = nullptr;
}

//TODO: smart algorithm of is_pow2
bool is_pow2(int x) {
    if (!x)
        return false;
    while (!(x & 1)) {
        x >>= 1;
    }
    return x == 1 || x == -1;
}

Nodes create_level(const Keys& keys) {
    assert(is_pow2(keys.size()));

    std::vector<Node*> nodes;
    for (auto key: keys) {
        if (key != NIL) {
            nodes.push_back(new Node(key));
        }
        else {
            nodes.push_back(nullptr);
        }
    }
    return nodes;
}

void bind_levels(Nodes& parents, Nodes& childs) {
    assert(2 * parents.size() == childs.size());
    int j = 0;
    for (auto p: parents) {
        if (p) {
            p->left = childs[j++];
            p->right = childs[j++];            
        }
        else {
            assert(!childs[j] && !childs[j+1]);
            j += 2;        
        }
    }
}

/* level_ctor
makes from {{0}, {1, 2}, {NIL, NIL, 5, 6}} tree
      0
  1       2
        5   6 
*/
Node* level_ctor(const std::vector<Keys>& levels_keys) {
    if (levels_keys.empty()) 
        return nullptr;
    assert(!levels_keys[0].empty());
    Nodes prev_level = create_level(levels_keys[0]);
    Node* root = prev_level[0];
    int height = levels_keys.size();
    for (int i = 1; i < height; ++i) {
        assert(!levels_keys[i].empty());
        auto curr_level = create_level(levels_keys[i]);
        bind_levels(prev_level, curr_level);
        prev_level = std::move(curr_level);
    }
    return root;
}

/* vdump
prints tree as:
    x
  2
    x
0
    x
  1
    x
*/
void vdump(const Node* root, std::ostream& os, int level = 0) {
    std::string tab(level * 2, ' ');
    if (!root) {
        os << tab << "x\n";
        return;
    }
    vdump(root->right, os, level + 1);
    os << tab << root->value << '\n';
    vdump(root->left, os, level + 1);
}

/* creates tree level by level.
A result of make_tree(6) is tree:
      0
  1       2
3   4   5   
*/
//TODO: use level_ctor
Node* make_tree(int size) {
    if (size <= 0)
        return nullptr;
    int count = 0;
    Node* root = new Node(count++);
    std::queue<Node*> queue_;
    queue_.push(root);
    for (;;) {
        Node* curr = queue_.front();
        queue_.pop();
        if (count < size) {
            Node* left = new Node(count++);
            queue_.push(left);
            curr->left = left;
        }
        else {
            break;
        }
        if (count < size) {
            Node* right = new Node(count++);
            queue_.push(right);
            curr->right = right;
        }
        else {
            break;
        }
    }        
    return root;
}

size_t depth(Node* root) {
    if (root) {
        size_t left_depth = depth(root->left);
        size_t right_depth = depth(root->right);
        return std::max(left_depth, right_depth) + 1;
    }
    else {
        return 0;
    }
}

//A tree is balanced if for every node in the tree
//depth of left and right subtree differs less than 2
bool isBalanced(Node* root) {
    if (root) {
        //current node balanced if depth of left and right
        //nodes differs less than 2
        ssize_t left_depth = depth(root->left);
        ssize_t right_depth = depth(root->right);
        ssize_t diff = std::abs(right_depth - left_depth);
        bool node_is_balanced = diff < 2;
        //check for all nodes in tree
        return node_is_balanced 
                && isBalanced(root->left)
                && isBalanced(root->right);
    }
    else {
        return true;
    }
}

int main() {
    std::vector<Keys> levels_keys = {
                {0}, 
            {1,     2}, 
        {4 , NIL, 5, 6}
    };
    Node* root = level_ctor(levels_keys);
    //destroy(root->right->left);
    vdump(root, std::cout);

    destroy(root);    
    return 0;
}
