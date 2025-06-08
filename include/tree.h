// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cstdint>

class PMTree {
 public:
    struct Node {
        char value;                     // символ, хранящийся в узле
        std::vector<Node*> children;    // потомки (отсортированы по возрастанию)
        explicit Node(char v) : value(v) {}
        ~Node() { for (auto* ch : children) delete ch; }
    };

    explicit PMTree(const std::vector<char>& alphabet) {
        std::vector<char> sorted = alphabet;
        std::sort(sorted.begin(), sorted.end());
        root = new Node('*');
        buildSubtree(root, sorted);
    }

    ~PMTree() { delete root; }

    // доступ к корню
    Node* getRoot() const { return root; }

 private:
    Node* root;

    // рекурсивное построение
    static void buildSubtree(Node* parent, const std::vector<char>& remaining) {
        if (remaining.empty()) return;
        for (size_t i = 0; i < remaining.size(); ++i) {
            std::vector<char> next = remaining;
            char pick = next[i];
            next.erase(next.begin() + i);
            auto* child = new Node(pick);
            parent->children.push_back(child);
            buildSubtree(child, next);
        }
    }
};

std::vector<std::vector<char>> getAllPerms(PMTree& tree);
std::vector<char> getPerm1(PMTree& tree, uint64_t num);
std::vector<char> getPerm2(PMTree& tree, uint64_t num);

#endif  // INCLUDE_TREE_H_
