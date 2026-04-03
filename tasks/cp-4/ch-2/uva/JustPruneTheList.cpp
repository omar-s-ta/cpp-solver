#include <iostream>
#include <unordered_set>
#include <vector>

template <typename T>
inline std::istream& operator>>(std::istream& in, std::vector<T>& ts) {
  for (T& t : ts) in >> t;
  return in;
}

class problem {
 public:
  void solveOne() {
    int n, m;
    in >> n >> m;

    std::vector<int> a(n), b(m);
    in >> a >> b;

    std::unordered_set<int> values(a.begin(), a.end());
    values.insert(b.begin(), b.end());

    std::unordered_multiset<int> all_a(a.begin(), a.end());
    std::unordered_multiset<int> all_b(b.begin(), b.end());

    int r = 0;
    for (const auto v : values) {
      int ca = all_a.count(v);
      int cb = all_b.count(v);
      r += abs(ca - cb);
    }

    out << r << '\n';
  }

  void solve() {
    int nt;
    in >> nt;
    for (int _ = 1; _ <= nt; _++) {
      solveOne();
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
