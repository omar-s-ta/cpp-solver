#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

const size_t MOD = 1000;

struct disjoint_set {
  std::vector<size_t> parent;
  std::vector<size_t> dist;  // dist from i -> parent[i]

  disjoint_set(const size_t n) : parent(n), dist(n, 0) {
    for (size_t i = 0; i < n; i++) {
      parent[i] = i;
    }
  }

  void join(size_t a, size_t b) {
    parent[a] = b;
    dist[a] = (a < b ? b - a : a - b) % MOD;
  }

  size_t find(size_t i) {
    if (parent[i] != i) {
      const size_t p = parent[i];
      const size_t r = find(p);
      dist[i] += dist[p];
      parent[i] = r;
    }
    return parent[i];
  }

  size_t set_size(size_t i) {
    find(i);
    return dist[i];
  }
};

/*
 * union_find with a trick.
 */
class problem {
 public:
  void solve_one() {
    i32 n;
    in >> n;

    disjoint_set s(n);
    for (char ch; in >> ch && ch != 'O';) {
      if (ch == 'E') {
        size_t i;
        in >> i;
        out << s.set_size(i - 1) << '\n';
      } else {
        size_t a, b;
        in >> a >> b;
        s.join(a - 1, b - 1);
      }
    }
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
