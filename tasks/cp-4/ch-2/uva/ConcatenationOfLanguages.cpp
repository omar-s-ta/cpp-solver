#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

class problem {
 public:
  void solve() {
    int nt;
    in >> nt;
    for (int _ = 1; _ <= nt; _++) {
      int n, m;
      in >> n >> m;
      in.ignore();

      std::vector<std::string> a(n), b(m);
      for (auto& t : a) std::getline(in, t);
      for (auto& t : b) std::getline(in, t);

      std::unordered_set<std::string> s;
      for (const auto& aa : a) {
        for (const auto& bb : b) {
          s.emplace(aa + bb);
        }
      }

      out << "Case " << _ << ": " << s.size() << '\n';
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
