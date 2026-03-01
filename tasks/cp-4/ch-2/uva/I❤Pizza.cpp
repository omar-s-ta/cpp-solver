#include <cstdint>
#include <iostream>
#include <unordered_map>

class problem {
 public:
  void solve() {
    int nt;
    in >> nt;
    for (std::string pizza, chs = "MARGIT"; nt-- != 0;) {
      in >> pizza;

      std::unordered_map<char, uint16_t> f;
      for (const char ch : pizza) {
        f[ch] += 1;
      }

      f['A'] /= 3;
      f['R'] /= 2;

      uint16_t r = 1000;
      for (const char ch : chs) {
        r = std::min(r, f[ch]);
      }
      out << r << '\n';
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
