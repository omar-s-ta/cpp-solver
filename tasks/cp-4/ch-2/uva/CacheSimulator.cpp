#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using namespace std;

using address = u32;
using instant = size_t;

const size_t N = 10000002;
const size_t R = (1 << 24) + 1;
const size_t C = 32;

array<u32, N> ft;
array<size_t, R> last_seen;
array<u32, C> cache_lens;
array<i32, C> cache_misses;

void ft_update(size_t i, const i32 value) {
  for (; i < N; i |= i + 1) {
    ft[i] += value;
  }
}

// [0, j]
u32 ft_rsq(size_t i) {
  u32 sum = 0;
  for (i32 j = i; j >= 0; j = (j & (j + 1)) - 1) {
    sum += ft[j];
  }
  return sum;
}

// [i, j]
u32 ft_rsq(size_t i, size_t j) { return ft_rsq(j) - ft_rsq(i - 1); }

/*
 * We will try to simulate a `LRU` cache, but not with a deque or hashmap, because of
 * the problem constraints.
 *
 * Assume we have an address 'a' that we reference on time 't'.
 * Assume we have 'lt' the last time we referenced address 'a'.
 *
 * let's have `d(t) = number of distinct addresses referenced strictly between 'lt' and 't'.`
 * - if 'a' has never been seen before, 'dt = infinity'.
 * - for a 'LRU' cache of capacity 'C':
 *   * the reference is a hit  'iff' 'd(t) <  C'
 *   * the reference is a miss 'iff' 'd(t) >= C'
 *
 * - We can use a fenwick tree to get 'd(t)'
 *
 * Since the `cache_lens`, the cache sizes given in the problem statement are
 * given in increasing order (sorted). We can on each address reference, get
 * the lower_bound on the cache sizes for each 'd(t)' and manipulate a 'prefix_sum'
 * for the `cache_misses` for every cache size.
 */
i32 main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  for (size_t caches; cin >> caches; ft.fill(0), last_seen.fill(0)) {
    for (size_t i = 0; i < caches; i++) {
      cin >> cache_lens[i];
    }

    instant ct = 1;
    for (string cmd; cin >> cmd && cmd[0] != 'E';) {
      if (cmd[0] == 'S') {
        i32 result = 0;
        for (size_t i = 0; i < caches; i++) {
          result += cache_misses[i];
          if (i > 0) {
            cout << ' ';
          }
          cout << result;
        }
        cout << '\n';
        for (size_t i = 0; i <= caches; i++) {
          cache_misses[i] = 0;
        }
        continue;
      }

      i32 b, y = 0, n = 1;
      cin >> b;
      if (cmd[0] == 'R') {
        cin >> y >> n;
      }

      for (i32 k = 0; k < n; k++) {
        address a = y * k + b;
        u32 dt = numeric_limits<u32>::max();

        if (last_seen[a]) {
          instant lt = last_seen[a];
          dt = ft_rsq(lt + 1, ct);
          ft_update(lt, -1);
        }
        ft_update(ct, +1);
        last_seen[a] = ct;
        ct += 1;

        auto at = upper_bound(cache_lens.begin(), cache_lens.begin() + caches, dt);
        cache_misses[0] += 1;
        cache_misses[distance(cache_lens.begin(), at)] -= 1;
      }
    }
  }

  return 0;
}
