#include <array>
#include <deque>
#include <iostream>
#include <list>
#include <string>
#include <vector>

namespace io {

template <typename T>
inline std::istream& operator>>(std::istream& in, std::vector<T>& ts) {
  for (T& t : ts) in >> t;
  return in;
}
template <typename T>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T>& ts) {
  const std::size_t n = ts.size();
  for (std::size_t i = 0; i < n; i++) {
    if (i > 0) out << ' ';
    out << ts[i];
  }
  return out;
}

template <typename T>
inline std::istream& operator>>(std::istream& in, std::deque<T>& ts) {
  for (T& t : ts) in >> t;
  return in;
}
template <typename T>
inline std::ostream& operator<<(std::ostream& out, const std::deque<T>& ts) {
  const std::size_t n = ts.size();
  for (std::size_t i = 0; i < n; i++) {
    if (i > 0) out << ' ';
    out << ts[i];
  }
  return out;
}

template <typename T, std::size_t _t_size>
inline std::istream& operator>>(std::istream& in, std::array<T, _t_size>& ts) {
  for (T& t : ts) in >> t;
  return in;
}
template <typename T, std::size_t _t_size>
inline std::ostream& operator<<(std::ostream& out, const std::array<T, _t_size>& ts) {
  for (std::size_t i = 0; i < _t_size; i++) {
    if (i > 0) out << ' ';
    out << ts[i];
  }
  return out;
}

template <typename U, typename V>
std::istream& operator>>(std::istream& in, std::pair<U, V>& p) {
  return in >> p.first >> p.second;
}
template <typename U, typename V>
std::ostream& operator<<(std::ostream& out, const std::pair<U, V>& p) {
  return out << p.first << ' ' << p.second;
}

#if __cplusplus >= 201703L
template <typename... Ts>
inline void std_read(std::istream& in, Ts&... ts) {
  (in >> ... >> ts);
}
#endif
}  // namespace io

using namespace io;

/**
 * One of the rare problems where you need think about
 * which linear data-structure is suitable here.
 * You want to avoid:
 *   1. linear time string concatenation.
 *   2. linear time list concatenation.
 */
class problem {
 public:
  void solve() {
    int n;
    in >> n;
    std::vector<std::string> strs(n);
    in >> strs;

    std::vector<std::list<int>> lists(n);
    for (int i = 0; i < n; i++) {
      lists[i].emplace_back(i);
    }

    int at = 0;
    int sizes = 0;

    for (int a, b; --n != 0;) {
      in >> a >> b;
      a -= 1;
      b -= 1;

      lists[a].splice(lists[a].end(), lists[b]);
      if (lists[a].size() > sizes) {
        sizes = lists[a].size();
        at = a;
      }
    }

    for (const int index : lists[at]) {
      out << strs[index];
    }
    out << '\n';
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
