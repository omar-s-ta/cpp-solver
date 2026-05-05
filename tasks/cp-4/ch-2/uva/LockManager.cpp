#include <algorithm>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class problem {
 public:
  void solve_one() {
    std::unordered_set<uint32_t> ignored;
    std::unordered_map<uint32_t, std::vector<lock_t>> locks;

    for (char sign; in >> sign && sign != '#';) {
      uint32_t tr_id, item;
      in >> tr_id >> item;

      if (ignored.count(tr_id)) {
        out << "IGNORED";
      } else {
        const lock_t wanted{tr_id, sign == 'S'};
        bool denied =
            std::any_of(locks[item].begin(), locks[item].end(),
                        [&wanted](const lock_t& other) { return other.conflicts(wanted); });

        if (denied) {
          ignored.emplace(tr_id);
          out << "DENIED";
        } else {
          locks[item].emplace_back(wanted);
          out << "GRANTED";
        }
      }

      out << '\n';
    }
  }

  void solve() {
    int nt;
    in >> nt;
    for (int _ = 0; _ < nt; _++) {
      if (_ > 0) {
        out << '\n';
      }
      solve_one();
    }
  }

  problem(std::istream& in, std::ostream& out) : in(in), out(out) {}

 private:
  struct lock_t {
    uint32_t tr_id;
    bool shared;

    bool conflicts(const lock_t& other) const {
      return tr_id != other.tr_id && (!shared || !other.shared);
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
