#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

template <typename T>
inline std::istream& operator>>(std::istream& in, std::vector<T>& ts) {
  for (T& t : ts) in >> t;
  return in;
}
template <typename T>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T>& ts) {
  const std::size_t n = ts.size();
  for (std::size_t i = 0; i < n; i++) {
    if (i > 0) out << ' ';
    out << ts[i];
  }
  return out;
}

class problem {
 public:
  void solveOne() {
    int m, n;
    in >> m >> n;

    std::vector<std::string> s(m);
    in >> s;

    std::vector<std::map<char, uint16_t>> a(n);
    for (int i = 0; i < m; i++) {
      for (size_t j = 0; j < s[i].size(); j++) {
        a[j][s[i][j]] += 1;
      }
    }

    int r = 0;
    for (const auto& mp : a) {
      int v = 0;
      char ch = '-';
      for (const auto& p : mp) {
        if (p.second > v) {
          v = p.second;
          ch = p.first;
        }
      }
      out << ch;
      r += m - v;
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
