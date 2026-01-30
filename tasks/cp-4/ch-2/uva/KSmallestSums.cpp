#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>

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

/*
 * minimize the problem be solved for:
 *  What are the min/max k sums I can get from two sorted arrays 'a' and 'b'?
 *  f(a[i] + b[j]) where 'f' is the applied function.
 */
class problem {
 public:
  void solve() {
    for (size_t n; in >> n;) {
      std::vector<std::vector<uint32_t>> rows(2, std::vector<uint32_t>(n));
      in >> rows.front();
      std::sort(rows.front().begin(), rows.front().end());

      // O(n^2 log(n))
      for (size_t i = 1; i < n; i++) {
        in >> rows.back();
        std::sort(rows.back().begin(), rows.back().end());

        std::priority_queue<node> q;
        // s(i, 0)
        for (size_t j = 0; j < n; j++) {
          q.emplace(rows.front()[j] + rows.back()[0], 0);
        }

        for (size_t j = 0; j < n; j++) {
          const auto state = q.top();
          q.pop();
          // a[i] + b[j] = s(i, j)
          rows.front()[j] = state.sum;
          // a[i] = s(i, j) - b[j]
          // probable next min sum = a[i] + b[j+1]
          q.emplace(rows.front()[j] - rows.back()[state.at] + rows.back()[state.at + 1],
                    state.at + 1);
        }
      }

      out << rows.front() << '\n';
    }
  }

  problem(std::istream& in, std::ostream& out) : in(in), out(out) {}

 private:
  struct node {
    // a[i] + b[j]
    uint32_t sum;
    // j
    size_t at;

    node(uint32_t s, size_t a) : sum(s), at(a) {}

    bool operator<(const node& other) const { return sum > other.sum; }
  };

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
