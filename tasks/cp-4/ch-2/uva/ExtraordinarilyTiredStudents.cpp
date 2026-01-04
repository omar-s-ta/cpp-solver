#include <array>
#include <cstddef>
#include <deque>
#include <iostream>
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

/*
 * The problem boundaries are very small. Just simulate
 * the 'awake' and 'asleep' periods.
 */
class problem {
 public:
  void solve() {
    for (int n, i = 1; in >> n && n != 0; i++) {
      std::vector<student_t> students(n);
      in >> students;

      int result = -1;
      for (int period = 1; period <= N; period++) {
        int awakes = 0;
        int asleep = 0;
        for (const auto& student : students) {
          if (student.is_awake())
            awakes += 1;
          else
            asleep += 1;
        }

        if (awakes == n) {
          result = period;
          break;
        }

        for (auto& student : students) {
          student.try_to_sleep(awakes, asleep);
        }
      }
      out << "Case " << i << ": " << result << '\n';
    }
  }

  problem(std::istream& in, std::ostream& out) : in(in), out(out) {}

 private:
  std::istream& in;
  std::ostream& out;

  const int N = 1000;

  struct student_t {
    std::vector<bool> awake;
    std::size_t at;

    bool is_awake() const { return awake[at]; }
    std::size_t next_moment() const { return (at + 1) % awake.size(); }
    void stay_awake() { at = 0; }

    void try_to_sleep(int awakes, int asleep) {
      bool wake_up = awake[at] && !awake[next_moment()];
      if (wake_up) {
        if (awakes < asleep) {
          at = next_moment();
        } else {
          stay_awake();
        }
      } else {
        at = next_moment();
      }
    }

    friend std::istream& operator>>(std::istream& is, student_t& student) {
      int a, b, c;
      is >> a >> b >> c;

      for (int i = 0; i < a; i++) {
        student.awake.push_back(true);
      }
      for (int i = 0; i < b; i++) {
        student.awake.push_back(false);
      }
      student.at = c - 1;
      return is;
    }
  };
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  problem solver(std::cin, std::cout);
  solver.solve();
  return 0;
}
