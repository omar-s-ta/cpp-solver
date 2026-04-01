#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <unordered_set>

class problem {
 public:
  void solve() {
    for (int n; in >> n && n != 0;) {
      out << r[s.count(n) > 0] << '\n';
    }
  }

  problem(std::istream& in, std::ostream& out) : in(in), out(out) {
    s.insert({1,        64,       729,      4096,     15625,    46656,    117649,
              262144,   531441,   1000000,  1771561,  2985984,  4826809,  7529536,
              11390625, 16777216, 24137569, 34012224, 47045881, 64000000, 85766121});
  }

 private:
  std::istream& in;
  std::ostream& out;

  std::unordered_set<uint32_t> s;
  std::array<std::string, 2> r = {"Ordinary", "Special"};
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  for (uint32_t n; std::cin >> n && n != 0;) {
    uint32_t root = std::round(std::pow(n, 1.0 / 6));
    bool special = false;
    for (uint32_t r = std::max(root - 1, uint32_t(0)); r <= root + 1; r++) {
      uint32_t p = r * r * r * r * r * r;
      if (p == n) {
        special = true;
        break;
      }
    }
    std::cout << (special ? "Special" : "Ordinary") << '\n';
  }

  return 0;
}
