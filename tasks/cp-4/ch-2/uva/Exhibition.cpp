#include <cstdint>
#include <iomanip>
#include <ios>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class problem {
 public:
  void solve_one(const uint32_t test_case) {
    uint32_t n;
    in >> n;

    std::unordered_map<uint32_t, uint32_t> owner_count;
    std::vector<std::unordered_set<uint32_t>> stamps(n);

    for (uint32_t i = 0; i < n; i++) {
      uint32_t m;
      in >> m;

      for (uint32_t v; m != 0; m--) {
        in >> v;
        if (stamps[i].insert(v).second) {
          owner_count[v] += 1;
        }
      }
    }

    uint32_t uniq = 0;
    std::vector<uint32_t> count_per_friend(n, 0);
    for (uint32_t i = 0; i < n; i++) {
      for (const uint32_t v : stamps[i]) {
        if (owner_count[v] == 1) {
          uniq += 1;
          count_per_friend[i] += 1;
        }
      }
    }

    out << "Case " << test_case << ':';
    for (uint32_t i = 0; i < n; i++) {
      double p = uniq == 0 ? 0.0 : 100.0 * count_per_friend[i] / double(uniq);
      out << ' ' << std::fixed << std::setprecision(6) << p << '%';
    }
    out << '\n';
  }

  void solve() {
    uint32_t nt;
    in >> nt;
    for (uint32_t _ = 1; _ <= nt; _++) {
      solve_one(_);
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
