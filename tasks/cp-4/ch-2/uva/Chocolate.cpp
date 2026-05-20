#include <cstdint>
#include <cstring>
#include <iostream>
#include <unordered_map>

/*
 * Since they are only 3 persons. You can simulate the ownership with a mask.
 * Count the mask ownerships and print these values.
 */
class problem {
 public:
  void solve_one() {
    int boys[N];
    for (int i = 0; i < N; i++) {
      in >> boys[i];
    }

    std::unordered_map<int, uint8_t> mask;
    for (int i = 0; i < N; i++) {
      for (int j = 0, tpe; j < boys[i]; j++) {
        in >> tpe;
        mask[tpe] |= (1u << i);
      }
    }

    int count[1 << 3] = {};
    for (const auto& p : mask) {
      count[p.second] += 1;
    }

    out << count[1] << ' ' << count[6] << '\n';
    out << count[2] << ' ' << count[5] << '\n';
    out << count[4] << ' ' << count[3] << '\n';
  }

  void solve() {
    int nt;
    in >> nt;
    for (int t = 1; t <= nt; t++) {
      out << "Case #" << t << ":\n";
      solve_one();
    }
  }

  problem(std::istream& in, std::ostream& out) : in(in), out(out) {}

 private:
  std::istream& in;
  std::ostream& out;

  static constexpr const size_t N = 3;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  problem solver(std::cin, std::cout);
  solver.solve();
  return 0;
}
