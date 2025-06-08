// Copyright 2022 NNTU-CS
#include  <iostream>
#include <numeric>
#include <unordered_map>
#include <cstdint>
#include  <fstream>
#include  <locale>
#include  <cstdlib>
#include  "tree.h"

namespace {
    uint64_t fact(size_t n)
    {
        static std::vector<uint64_t> f{1};
        while (f.size() <= n) f.push_back(f.back() * f.size());
        return f[n];
    }

    void dfs(PMTree::Node* node, std::vector<char>& path,
             std::vector<std::vector<char>>& out)
    {
        if (node->children.empty()) { out.push_back(path); return; }
        for (auto* ch : node->children) {
            path.push_back(ch->value);
            dfs(ch, path, out);
            path.pop_back();
        }
    }

    // рекурсивная навигация для getPerm2
    bool navigate(PMTree::Node* node, uint64_t idx,          // 0-based
                  std::vector<char>& path)
    {
        if (node->children.empty()) return idx == 0;         // лист
        size_t branchSize = fact(path.capacity() - path.size() - 1);
        for (auto* ch : node->children) {
            if (idx < branchSize) {
                path.push_back(ch->value);
                return navigate(ch, idx, path);
            }
            idx -= branchSize;
        }
        return false;                                        // не существует
    }
} // namespace

std::vector<std::vector<char>> getAllPerms(PMTree& tree)
{
    std::vector<std::vector<char>> res;
    std::vector<char> path;
    size_t depth = 0, tmp = tree.getRoot()->children.size();
    while (tmp) { ++depth; tmp--; }
    path.reserve(depth);
    dfs(tree.getRoot(), path, res);
    return res;
}

std::vector<char> getPerm1(PMTree& tree, uint64_t num)
{
    if (num == 0) return {};
    auto perms = getAllPerms(tree);
    if (num > perms.size()) return {};
    return perms[num - 1];
}

std::vector<char> getPerm2(PMTree& tree, uint64_t num)
{
    --num;                          // переводим к 0-based
    size_t n = tree.getRoot()->children.size();
    uint64_t total = fact(n);
    if (num >= total) return {};

    std::vector<char> result;
    result.reserve(n);
    if (navigate(tree.getRoot(), num, result))
        return result;
    return {};
}
