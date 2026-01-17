#include <algorithm>
#include <charconv>
#include <cstddef>
#include <deque>
#include <iostream>
#include <string>
#include <string_view>

namespace io {

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const std::deque<T>& ts) {
  const std::size_t n = ts.size();
  out << '[';
  for (std::size_t i = 0; i < n; i++) {
    if (i > 0) out << ',';
    out << ts[i];
  }
  out << ']';
  return out;
}

#if __cplusplus >= 201703L
template <typename... Ts>
inline void std_read(std::istream& in, Ts&... ts) {
  (in >> ... >> ts);
}
#endif
}  // namespace io

using namespace io;

/*
 * Use a double ended queue to avoid reversing on each 'R' operation.
 * I learned about `std::string_view`, my first official usage.
 */
class problem {
 public:
  void solveOne() {
    std::string p, vs;
    std::size_t n;
    std_read(in, p, n, vs);

    std::deque<int> q = int_list(vs);

    bool rev = false;
    for (const auto& ch : p) {
      if (ch == 'D') {
        if (q.empty()) {
          out << "error" << '\n';
          return;
        }
        rev ? q.pop_back() : q.pop_front();
      } else {
        rev = !rev;
      }
    }
    if (rev) {
      std::ranges::reverse(q);
    }
    out << q << '\n';
  }

  void solve() {
    std::size_t nt;
    in >> nt;
    for (std::size_t _ = 1; _ <= nt; _++) {
      solveOne();
    }
  }

  problem(std::istream& in, std::ostream& out) : in(in), out(out) {}

 private:
  std::istream& in;
  std::ostream& out;

  std::deque<int> int_list(std::string_view vs) {
    vs.remove_prefix(1);
    vs.remove_suffix(1);

    std::deque<int> ns;
    auto b = vs.data();
    auto e = vs.data() + vs.size();

    while (b < e) {
      int v;
      auto [ptr, _] = std::from_chars(b, e, v);
      ns.emplace_back(v);
      b = ptr + 1;
    }
    return ns;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  problem solver(std::cin, std::cout);
  solver.solve();
  return 0;
}
