#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class problem {
 public:
  void solve_one(const int ti) {
    std::unordered_map<std::string, uint32_t> count;
    std::vector<std::string> words;

    for (std::string line; std::getline(in, line) && line != "END";) {
      for (char& ch : line) {
        if (!is_lower(ch)) {
          ch = ' ';
        }
      }

      std::stringstream ss(line);
      for (std::string word; ss >> word;) {
        count[word];
        words.emplace_back(word);
      }
    }

    const size_t distinct = count.size();

    uint32_t l = 0, r = words.size() - 1;
    size_t size = 0;

    for (size_t rgt = 0, lft = 0; rgt < words.size(); rgt++) {
      uint32_t& freq = count[words[rgt]];
      freq += 1;

      if (freq == 1) {
        size += 1;
      }

      while (size == distinct) {
        if (rgt - lft < r - l) {
          l = lft;
          r = rgt;
        }
        if (--count[words[lft]] == 0) {
          size -= 1;
        }
        lft += 1;
      }
    }

    out << "Document " << ti << ": " << l + 1 << ' ' << r + 1 << '\n';
  }

  void solve() {
    int nt;
    in >> nt;
    in.ignore();
    for (int _ = 1; _ <= nt; _++) {
      solve_one(_);
    }
  }

  problem(std::istream& in, std::ostream& out) : in(in), out(out) {}

 private:
  bool is_lower(const char ch) { return std::islower(static_cast<unsigned char>(ch)); }

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
