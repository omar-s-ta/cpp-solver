#include <bitset>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using std::cin;
using std::cout;

const size_t N = 10000;

struct disjoint_set {
  disjoint_set(const size_t n) : pr(n), sz(n, 1) {
    std::iota(pr.begin(), pr.end(), 0);
    relation.reset();
  }

  std::pair<i32, bool> find(u32 i) {
    if (pr[i] == i) {
      return {i, false};
    }
    auto p = find(pr[i]);
    pr[i] = p.first;
    relation[i] = relation[i] ^ p.second;
    return {pr[i], relation[i]};
  }

  bool join(u32 a, u32 b, u32 is_enemies) {
    auto pa = find(a);
    auto pb = find(b);
    auto ra = pa.first, rb = pb.first;
    auto ea = pa.second, eb = pb.second;
    if (ra == rb) {
      return (ea ^ eb) == is_enemies;
    }
    if (sz[ra] < sz[rb]) {
      std::swap(ra, rb);
    }
    sz[ra] += sz[rb];
    pr[rb] = ra;
    relation[rb] = ea ^ eb ^ is_enemies;
    return true;
  }

  bool is_friends(u32 a, u32 b) {
    auto pa = find(a);
    auto pb = find(b);
    return pa.first == pb.first && pa.second == pb.second;
  }

  bool is_enemies(u32 a, u32 b) {
    auto pa = find(a);
    auto pb = find(b);
    return pa.first == pb.first && pa.second != pb.second;
  }

 private:
  std::vector<i32> pr;
  std::vector<u32> sz;
  std::bitset<N> relation;
};

int main() {
  std::ios::sync_with_stdio(false);
  cin.tie(nullptr);

  for (u32 n, c, a, b; cin >> n;) {
    disjoint_set s(n);

    while (cin >> c >> a >> b && c + a + b != 0) {
      switch (c) {
        case 1:
          if (!s.join(a, b, 0)) {
            cout << "-1\n";
          }
          break;
        case 2:
          if (!s.join(a, b, 1)) {
            cout << "-1\n";
          }
          break;
        case 3:
          cout << s.is_friends(a, b) << '\n';
          break;
        case 4:
          cout << s.is_enemies(a, b) << '\n';
          break;
        default:
          break;
      }
    }
  }

  return 0;
}
