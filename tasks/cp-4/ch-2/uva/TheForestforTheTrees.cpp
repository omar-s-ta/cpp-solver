#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

static constexpr size_t V = 26;

class problem {
 public:
  void solve_one() {
    std::array<std::vector<u32>, V> adj{};

    for (std::string edge; in >> edge && edge[0] != '*';) {
      const size_t f = edge[1] - 'A';
      const size_t t = edge[3] - 'A';
      adj[f].emplace_back(t);
      adj[t].emplace_back(f);
    }

    std::string vertices;
    std::array<bool, V> visited{};
    in >> vertices;

    const u32 acorns = std::count_if(vertices.begin(), vertices.end(), [&](const char ch) {
      bool pred = ch >= 'A' && ch <= 'Z' && adj[ch - 'A'].empty();
      if (pred) {
        visited[ch - 'A'] = true;
      }
      return pred;
    });

    u32 trees = 0;
    for (u32 v = 0; v < V; v++) {
      if (visited[v] || adj[v].empty()) {
        continue;
      }
      dfs(v, visited, adj);
      // invariant `nodes == edges + 1` from problem description.
      // The problem guarantees a forest, so every connected component is 'acyclic'.
      // every component with '>= 1 edges' is a tree
      // So nodes = edges + 1
      trees += 1;
    }

    out << "There are " << trees << " tree(s) and " << acorns << " acorn(s).\n";
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

  void dfs(u32 u, std::array<bool, ::V>& visited, const std::array<std::vector<u32>, V>& adj) {
    visited[u] = true;
    for (const u32 v : adj[u]) {
      if (!visited[v]) {
        dfs(v, visited, adj);
      }
    }
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  problem solver(std::cin, std::cout);
  solver.solve();
  return 0;
}
