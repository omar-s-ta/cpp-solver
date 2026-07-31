#include <cstddef>
#include <cstdint>
#include <iostream>
#include <tuple>
#include <vector>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

struct disjoint_set {
  std::vector<size_t> parent;
  std::vector<size_t> size;
  std::vector<size_t> sum;
  std::vector<size_t> at;
  size_t next;

  disjoint_set(const size_t n, const size_t m)
      : parent(n + m), size(n + m, 1), sum(n + m), at(n + m), next(n) {
    for (size_t i = 0; i < n + m; i++) {
      parent[i] = i;
      at[i] = i;
      sum[i] = i;
    }
  }

  void join(size_t a, size_t b) { unite(root(a), root(b)); }

  void unite(size_t a, size_t b) {
    if (a == b) {
      return;
    }
    parent[b] = a;
    size[a] += size[b];
    sum[a] += sum[b];
  }

  void move(size_t f, size_t t) {
    const size_t p = root(f);
    const size_t q = root(t);
    if (p == q) {
      return;
    }
    size[p] -= 1;
    sum[p] -= f;
    size[next] = 1;
    sum[next] = f;
    at[f] = next;
    unite(next, q);
    next += 1;
  }

  size_t find(size_t i) {
    if (parent[i] != i) {
      parent[i] = find(parent[i]);
    }
    return parent[i];
  }

  size_t root(size_t i) { return find(at[i]); }

  std::tuple<size_t, size_t> get(size_t i) {
    const size_t r = root(i);
    return std::make_tuple(size[r], sum[r]);
  }
};

class problem {
 public:
  void solve() {
    for (size_t n, m; in >> n >> m;) {
      disjoint_set s(n + 1, m);
      for (size_t c, a, b; m != 0; m--) {
        in >> c;
        if (c == 1) {
          in >> a >> b;
          s.join(a, b);
        } else if (c == 2) {
          in >> a >> b;
          s.move(a, b);
        } else {
          in >> c;
          std::tie(a, b) = s.get(c);
          out << a << ' ' << b << '\n';
        }
      }
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
