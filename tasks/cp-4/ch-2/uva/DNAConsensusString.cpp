#include <iostream>
#include <string>
#include <vector>

class problem {
 public:
  void solveOne() {
    int m, n;
    in >> m >> n;

    std::vector<std::string> s(m);
    for (auto& x : s) in >> x;

    const char dna[] = "ACGT";
    int r = 0;
    for (int j = 0; j < n; j++) {
      int cnt[4] = {};
      for (int i = 0; i < m; i++) {
        for (int k = 0; k < 4; k++) {
          if (s[i][j] == dna[k]) {
            cnt[k] += 1;
          }
        }
      }

      int best = 0;
      for (int k = 0; k < 4; k++) {
        if (cnt[k] > cnt[best]) {
          best = k;
        }
      }

      out << dna[best];
      r += m - cnt[best];
    }
    out << '\n' << r << '\n';
  }

  void solve() {
    int nt;
    in >> nt;
    while (nt-- != 0) {
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
