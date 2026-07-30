#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

struct disjoint_set {
  std::vector<i32> parent;

  disjoint_set(const size_t n) : parent(n, -1) {}

  bool join(size_t a, size_t b) {
    a = find(a);
    b = find(b);
    if (a == b) {
      return false;
    }
    if (parent[a] > parent[b]) {
      std::swap(a, b);
    }
    parent[a] = parent[a] + parent[b];
    parent[b] = a;
    return true;
  }

  size_t find(size_t i) {
    for (;;) {
      const i32 p = parent[i];
      if (p < 0) {
        return i;
      }
      const i32 gp = parent[p];
      if (gp < 0) {
        return p;
      }
      parent[i] = gp;
      i = gp;
    }
  }

  i32 size(size_t i) { return -parent[find(i)]; }
};

class problem {
 public:
  void solve() {
    for (size_t c, r; in >> c >> r && (c || r);) {
      std::unordered_map<std::string, size_t> id;
      for (size_t i = 0; i < c; i++) {
        std::string animal;
        in >> animal;
        id[animal] = i;
      }
      disjoint_set s(c);
      i32 result = 1;
      for (std::string a, b; r-- != 0;) {
        in >> a >> b;
        size_t ia = id[a], ib = id[b];
        if (s.join(ia, ib)) {
          result = std::max(result, s.size(ia));
        }
      }
      out << result << '\n';
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
