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

}


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
src/main.cpp
cpp

#include "tree.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using HighResClock = std::chrono::high_resolution_clock;

template <class F>
double Bench(F&& fn) {
  auto t0 = HighResClock::now();
  fn();
  auto dt = std::chrono::duration<double>(HighResClock::now() - t0);
  return dt.count() * 1e3;
}

int main() {
  std::vector<char> in = {'1', '2', '3'};
  PMTree tree(in);

  std::cout << "Все перестановки:\n";
  for (const auto& v : getAllPerms(tree)) {
    for (char c : v) std::cout << c;
    std::cout << '\n';
  }

  std::cout << "perm1(2) = ";
  for (char c : getPerm1(tree, 2)) std::cout << c;
  std::cout << "\nperm2(2) = ";
  for (char c : getPerm2(tree, 2)) std::cout << c;
  std::cout << '\n';

  std::mt19937 rng(std::random_device{}());
  std::ofstream csv("times.csv");
  csv << "n,getAllPerms,getPerm1,getPerm2\n";

  for (int n = 3; n <= 10; ++n) {  // 10! = 3 628 800
    std::vector<char> alpha;
    for (int i = 0; i < n; ++i) alpha.push_back('A' + i);

    PMTree t(alpha);

    uint64_t total = 1;
    for (int i = 2; i <= n; ++i) total *= i;

    std::uniform_int_distribution<uint64_t> dist(1, total);

    double t_all = Bench([&] { getAllPerms(t); });
    double t_p1  = Bench([&] { getPerm1(t, dist(rng)); });
    double t_p2  = Bench([&] { getPerm2(t, dist(rng)); });

    csv << n << ',' << t_all << ',' << t_p1 << ',' << t_p2 << '\n';
    std::cout << "n = " << n << " готово\n";
  }
}
