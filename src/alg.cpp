// Copyright 2022 NNTU-CS
#include  <iostream>
#include <cstdint>
#include <numeric>
#include <unordered_map>
#include <vector>
#include  <fstream>
#include  <locale>
#include  <cstdlib>
#include  "tree.h"


namespace {

uint64_t Fact(size_t n) {
  static std::vector<uint64_t> cache{1};
  while (cache.size() <= n) cache.push_back(cache.back() * cache.size());
  return cache[n];
}

void Dfs(PMTree::Node* node,
         std::vector<char>& path,
         std::vector<std::vector<char>>& out) {
  if (node->children.empty()) {
    out.push_back(path);
    return;
  }

  for (auto* ch : node->children) {
    path.push_back(ch->value);
    Dfs(ch, path, out);
    path.pop_back();
  }
}

bool Navigate(PMTree::Node* node,
              uint64_t idx,          // 0-based
              std::vector<char>& path) {
  if (node->children.empty()) return idx == 0;

  size_t rest = path.capacity() - path.size() - 1;
  uint64_t branch = Fact(rest);

  for (auto* ch : node->children) {
    if (idx < branch) {
      path.push_back(ch->value);
      return Navigate(ch, idx, path);
    }
    idx -= branch;
  }
  return false;  // не существует
}

}  // anonymous namespace


std::vector<std::vector<char>> getAllPerms(PMTree& tree) {
  std::vector<std::vector<char>> res;
  std::vector<char> path;
  size_t depth = tree.getRoot()->children.size();
  path.reserve(depth);

  Dfs(tree.getRoot(), path, res);
  return res;
}

std::vector<char> getPerm1(PMTree& tree, uint64_t num) {
  if (num == 0) return {};
  auto perms = getAllPerms(tree);
  if (num > perms.size()) return {};
  return perms[num - 1];
}

std::vector<char> getPerm2(PMTree& tree, uint64_t num) {
  if (num == 0) return {};

  size_t n = tree.getRoot()->children.size();
  uint64_t total = Fact(n);
  if (--num >= total) return {};

  std::vector<char> result;
  result.reserve(n);

  return Navigate(tree.getRoot(), num, result) ? result : std::vector<char>{};
}
