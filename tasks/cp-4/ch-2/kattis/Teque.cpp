#include <cassert>
#include <deque>
#include <iostream>
#include <string>
#include <utility>

template <class T>
struct teque {
  std::deque<T> a, b;

  template <class... Args>
  void emplace_front(Args&&... args) {
    a.emplace_front(std::forward<Args>(args)...);
    balance();
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    b.emplace_back(std::forward<Args>(args)...);
    balance();
  }

  template <class... Args>
  void emplace_middle(Args&&... args) {
    a.emplace_back(std::forward<Args>(args)...);
    balance();
  }

  const T& operator[](const std::size_t at) const {
    assert(at < a.size() + b.size());
    if (at < a.size()) {
      return a[at];
    }
    return b[at - a.size()];
  }

 private:
  inline void balance() {
    if (a.size() > b.size() + 1) {
      if (a.empty()) return;
      b.emplace_front(a.back());
      a.pop_back();
    } else if (a.size() < b.size()) {
      a.emplace_back(b.front());
      b.pop_front();
    }
  }
};

/*
 * Use two deques and preserve the state that `a.size() == b.size() || a.size() == b.size() + 1`
 */
class problem {
 public:
  void solve() {
    int T, v;
    in >> T;

    teque<int> t;
    for (std::string s; T--;) {
      in >> s >> v;
      if (s[0] == 'p') {
        const char ch = s.back();
        switch (ch) {
          case 't':
            t.emplace_front(v);
            break;
          case 'k':
            t.emplace_back(v);
            break;
          case 'e':
            t.emplace_middle(v);
            break;
        }
      } else {
        out << t[v] << '\n';
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
