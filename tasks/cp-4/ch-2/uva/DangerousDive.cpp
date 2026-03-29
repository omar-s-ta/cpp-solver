#include <iostream>
#include <vector>

class problem {
 public:
  void solve() {
    for (int n, m; in >> n >> m;) {
      std::vector<bool> v(n, false);
      for (int i = 0; i < m; i++) {
        int a;
        in >> a;
        v[a - 1] = true;
      }
      if (n == m) {
        out << '*';
      } else {
        for (int i = 0; i < n; i++) {
          if (!v[i]) {
            out << i + 1 << ' ';
          }
        }
      }
      out << '\n';
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
