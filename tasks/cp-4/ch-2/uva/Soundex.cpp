#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

class problem {
 public:
  void solve() {
    for (std::string w; in >> w;) {
      if (h[w.front()]) {
        out << h[w.front()];
      }
      for (size_t i = 1; i < w.size(); i++) {
        if (!h[w[i]] || h[w[i]] == h[w[i - 1]]) {
          continue;
        }
        out << h[w[i]];
      }
      out << '\n';
    }
  }

  problem(std::istream& in, std::ostream& out) : in(in), out(out) {
    h['B'] = h['F'] = h['P'] = h['V'] = 1;
    h['C'] = h['G'] = h['J'] = h['K'] = h['Q'] = h['S'] = h['X'] = h['Z'] = 2;
    h['D'] = h['T'] = 3;
    h['L'] = 4;
    h['N'] = h['M'] = 5;
    h['R'] = 6;
  }

 private:
  std::istream& in;
  std::ostream& out;

  std::unordered_map<char, uint16_t> h;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  problem solver(std::cin, std::cout);
  solver.solve();
  return 0;
}
