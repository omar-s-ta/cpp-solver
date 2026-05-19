#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>

class problem {
 public:
  void read(uint32_t n, std::priority_queue<int>& ps) {
    for (uint32_t i = 0; i < n; i++) {
      int p;
      in >> p;
      ps.emplace(p);
    }
  }

  void print(std::priority_queue<int>& ps) {
    for (; !ps.empty(); ps.pop()) {
      out << ps.top() << '\n';
    }
  }

  void solve_one() {
    uint32_t b, sb, sg;
    in >> b >> sg >> sb;

    std::priority_queue<int> bl, gr;
    read(sg, gr);
    read(sb, bl);

    while (!bl.empty() && !gr.empty()) {
      std::vector<int> bb, gg;
      for (uint32_t i = 0; i < b; i++) {
        if (bl.empty() || gr.empty()) {
          break;
        }

        int power_b = bl.top();
        int power_g = gr.top();

        bl.pop();
        gr.pop();

        if (power_b == power_g) {
          continue;
        }

        if (power_b > power_g) {
          bb.emplace_back(power_b - power_g);
        } else {
          gg.emplace_back(power_g - power_b);
        }
      }

      for (auto bv : bb) bl.emplace(bv);
      for (auto gv : gg) gr.emplace(gv);
    }

    if (bl.empty() && gr.empty()) {
      out << "green and blue died\n";
    } else if (bl.empty()) {
      out << "green wins\n";
      print(gr);
    } else {
      out << "blue wins\n";
      print(bl);
    }
  }

  void solve() {
    int nt;
    in >> nt;
    for (int _ = 1; _ <= nt; _++) {
      if (_ > 1) {
        out << '\n';
      }
      solve_one();
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
