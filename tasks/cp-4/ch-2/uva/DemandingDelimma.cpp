#include <array>
#include <cstdint>
#include <iostream>
#include <numeric>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

static constexpr size_t N = 10;

class problem {
 public:
  void solve_one() {
    u32 n, m;
    in >> n >> m;

    for (u32 i = 0; i < n; i++) {
      for (u32 j = 0; j < m; j++) {
        in >> adjm[j][i];
      }
    }

    bool incidence = true;
    for (u32 i = 0; i < m && incidence; i++) {
      u16 agg = std::accumulate(adjm[i].begin(), adjm[i].begin() + n, 0);
      incidence &= agg == 2;
    }

    for (u32 i = 0; i < m && incidence; i++) {
      for (u32 j = i + 1; j < m && incidence; j++) {
        bool equal = true;
        for (u32 k = 0; k < m && equal; k++) {
          equal &= adjm[i][k] == adjm[j][k];
        }
        incidence &= !equal;
      }
    }

    out << (incidence ? "Yes\n" : "No\n");
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

  std::array<std::array<u16, N>, N*(N - 1)> adjm;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  problem solver(std::cin, std::cout);
  solver.solve();
  return 0;
}
