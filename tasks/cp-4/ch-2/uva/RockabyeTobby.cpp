#include <cstdint>
#include <iostream>
#include <ostream>
#include <queue>
#include <string>

/*
 * Use a priority_queue to keep the order.
 * pop and push the new pill times until k times.
 */
class problem {
 public:
  void solveOne() {
    int n, k;
    in >> n >> k;

    std::priority_queue<medicine> q;
    for (int i = 0; i < n; i++) {
      std::string name;
      uint32_t moment;
      in >> name >> moment;
      q.emplace(name, moment, n - i);
    }

    while (k-- != 0 && !q.empty()) {
      auto pill = q.top();
      q.pop();
      out << pill << '\n';
      pill.upgrade();
      q.emplace(pill);
    }
  }

  void solve() {
    int nt;
    in >> nt;
    while (nt-- != 0) {
      solveOne();
    }
  }

  problem(std::istream& in, std::ostream& out) : in(in), out(out) {}

 private:
  struct medicine {
    std::string name;
    uint32_t moment;
    uint32_t priority;
    uint32_t init;

    medicine(const std::string& n, uint32_t m, uint32_t p)
        : name(n), moment(m), priority(p), init(m) {}

    void upgrade() { moment += init; }

    bool operator<(const medicine& other) const {
      if (moment != other.moment) return moment > other.moment;
      return priority < other.priority;
    }

    inline friend std::ostream& operator<<(std::ostream& o, const medicine& m) {
      return o << m.moment << ' ' << m.name;
    }
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
