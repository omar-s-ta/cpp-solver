#include <iostream>
#include <list>
#include <string>

/*
 * Use a list for O(1) insertion and deletion.
 */
class problem {
 public:
  void solveOne() {
    std::string s;
    std::getline(in, s);
    std::list<char> l;
    std::list<char>::iterator it = l.begin();
    for (const char c : s) {
      if (c == '[') {
        it = l.begin();
      } else if (c == ']') {
        it = l.end();
      } else if (c == '<') {
        if (it != l.begin()) {
          --it;
          it = l.erase(it);
        }
      } else {
        l.insert(it, c);
      }
    }
    for (const auto c : l) {
      out << c;
    }
    out << '\n';
  }

  void solve() {
    int nt;
    in >> nt;
    in.ignore();
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
