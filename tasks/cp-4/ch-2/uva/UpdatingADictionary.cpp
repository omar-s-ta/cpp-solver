#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using dictionary = std::map<std::string, std::string>;
using record = std::pair<std::string, std::string>;

class problem {
 public:
  template <typename Pred>
  void print(const dictionary& n, const char ch, Pred pred) {
    bool first = true;
    for (const auto& p : n) {
      if (pred(p)) {
        if (first) {
          out << ch;
          first = false;
        } else {
          out << ',';
        }
        out << p.first;
      }
    }
    if (!first) {
      out << '\n';
    }
  }

  dictionary parse(const std::string& s) {
    std::istringstream iss(s.substr(1, s.size() - 2));
    dictionary result;
    for (std::string kv; iss >> kv;) {
      const size_t at = kv.find(':');
      result[kv.substr(0, at)] = kv.substr(at + 1);
    }
    return result;
  }

  void solve_one() {
    std::string a, b;
    in >> a >> b;

    std::replace(a.begin(), a.end(), ',', ' ');
    std::replace(b.begin(), b.end(), ',', ' ');

    dictionary o = parse(a);
    dictionary n = parse(b);

    if (o == n) {
      out << "No changes" << '\n';
    } else {
      print(n, '+', [&](const record& p) { return !o.count(p.first); });
      print(o, '-', [&](const record& p) { return !n.count(p.first); });
      print(n, '*', [&](const record& p) {
        auto it = o.find(p.first);
        return it != o.end() && it->second != p.second;
      });
    }

    out << '\n';
  }

  void solve() {
    u32 nt;
    in >> nt;
    for (u32 _ = 1; _ <= nt; _++) {
      solve_one();
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
