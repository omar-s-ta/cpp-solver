#include <cstdint>
#include <iostream>
#include <set>

class problem {
 public:
  void solve_one(int n) {
    uint64_t cost = 0;
    std::multiset<uint32_t> urn;
    for (int bills; n-- != 0 && in >> bills;) {
      for (uint32_t bill; bills-- != 0 && in >> bill; urn.emplace(bill));
      cost += *urn.rbegin() - *urn.begin();
      urn.erase(urn.begin());
      urn.erase(--urn.end());
    }
    out << cost << '\n';
  }

  void solve() { for (int n; in >> n && n != 0; solve_one(n)); }

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
