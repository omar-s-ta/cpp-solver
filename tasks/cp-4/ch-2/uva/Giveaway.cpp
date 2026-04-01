#include <array>
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
  problem solver(std::cin, std::cout);
  solver.solve();
  return 0;
}
