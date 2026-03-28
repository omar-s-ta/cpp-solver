#include <cstddef>
#include <iostream>
#include <regex>
#include <string>

/*
 * Axion: xM?Ex?        -> be careful of the segments relation
 *        (a, 1, a + 1) -> where a is one or more '?'
 */
class problem {
 public:
  void solve() {
    int nt;
    in >> nt;

    std::regex rg(R"(\?+M\?+E\?+)");

    for (std::string s; nt-- != 0;) {
      in >> s;

      std::smatch rg_match;
      if (regex_match(s, rg_match, rg)) {
        size_t m = s.find('M');
        size_t e = s.find('E');

        size_t before = m;
        size_t middle = e - m - 1;
        size_t after = s.size() - e - 1;

        out << (after == before + middle ? "theorem" : "no-theorem");
      } else {
        out << "no-theorem";
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
