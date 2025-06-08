// Copyright 2022 NNTU-CS
#include <iostream>
#include <chrono>
#include <iomanip>
#include <random>
#include <fstream>
#include <vector>
#include "tree.h"

using HighResClock = std::chrono::high_resolution_clock;

template <class F>
double Bench(F&& fn) {
  auto t0 = HighResClock::now();
  fn();
  auto dt = std::chrono::duration<double>(HighResClock::now() - t0);
  return dt.count() * 1e3;  // миллисекунды
}

int main() {
  /* -------- мини-демо -------------------------------------- */
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

  /* -------- эксперимент ------------------------------------ */
  std::mt19937 rng(std::random_device {}());
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
