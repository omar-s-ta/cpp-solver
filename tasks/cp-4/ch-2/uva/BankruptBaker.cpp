#include <algorithm>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <istream>
#include <string>
#include <unordered_map>
#include <vector>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

inline char to_upper(const char ch) {
  return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
}

class problem {
 public:
  void solve_one() {
    std::string title, ingredient, recipe;
    in >> std::ws;
    std::getline(in, title);
    std::transform(title.begin(), title.end(), title.begin(), to_upper);

    u32 ingredients, recipes, budget;
    in >> ingredients >> recipes >> budget;

    std::unordered_map<std::string, u32> price;
    for (u32 cost; ingredients != 0; ingredients--) {
      in >> ingredient >> cost;
      price[ingredient] = cost;
    }

    std::vector<std::pair<u32, std::string>> total;
    total.reserve(recipes);

    for (u32 k; recipes != 0; recipes--) {
      in >> std::ws;
      std::getline(in, recipe);
      in >> k;
      u32 sum = 0;
      for (u32 amount; k != 0; k--) {
        in >> ingredient >> amount;
        sum += price[ingredient] * amount;
      }
      if (sum <= budget) {
        total.emplace_back(sum, std::move(recipe));
      }
    }

    out << title << '\n';
    if (total.empty()) {
      out << "Too expensive!\n";
    } else {
      std::sort(total.begin(), total.end());
      for (const auto& p : total) {
        out << p.second << '\n';
      }
    }
    out << '\n';
  }

  void solve() {
    u32 nt;
    in >> nt;
    for (u32 _ = 1; _ <= nt; _++) {
      solve_one();
    }
  }

  problem(std::istream& in, std::ostream& out) : in(in), out(out) {}

 private:
  std::istream& in;
  std::ostream& out;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  problem solver(std::cin, std::cout);
  solver.solve();
  return 0;
}
