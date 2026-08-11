#include <algorithm>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <vector>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using std::cin;
using std::cout;

struct fenwick_tree {
  struct all_ones_t {};

  i32 size() const { return i32(ft.size()); }

  fenwick_tree(const i32 n, all_ones_t) : ft(n, 0) {
    for (i32 i = 0; i < n; i++) {
      ft[i] += i32(i != 0);
      const i32 p = i | (i + 1);
      if (p < n) {
        ft[p] += ft[i];
      }
    }
  }

  void update(i32 i, i32 v) {
    for (int n = size(); i < n; i |= i + 1) {
      ft[i] += v;
    }
  }

  i32 rsq(i32 i) {
    i32 s = 0;
    for (; i >= 0; i = (i & (i + 1)) - 1) {
      s += ft[i];
    }
    return s;
  }

  i32 rsq(i32 i, i32 j) { return rsq(j) - rsq(i - 1); }

  i32 select(i32 k) {
    i32 n = size();
    i32 p = 1;
    while (p * 2 <= n) {
      p *= 2;
    }
    i32 i = 0;
    while (p != 0) {
      if (i + p - 1 < n) {
        const i32 c = ft[i + p - 1];
        if (c < k) {
          k -= c;
          i += p;
        }
      }
      p /= 2;
    }
    return i;
  }

 private:
  std::vector<i32> ft;
};

struct order_stat_set {
  order_stat_set(const i32 n) : ft(n, fenwick_tree::all_ones_t{}) {}

  i32 count(const i32 v) { return ft.rsq(v, v); }

  void insert(const i32 v) { ft.update(v, 1); }

  bool remove(const i32 v) {
    if (count(v) <= 0) {
      return false;
    }
    ft.update(v, -1);
    return true;
  }

  i32 kth(i32 k) { return ft.select(k); }

 private:
  fenwick_tree ft;
};

const i32 N = 2000000 + 1;

/*
 * Build an `order_stat_set` from a `fenwick_tree` to be able to get
 * the element in current position i.
 * Then pre-build the lucky numbers.
 */
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  i32 count = N - 1;
  order_stat_set s(N);

  for (i32 i = 2; i < N; i += 2) {
    s.remove(i);
    count -= 1;
  }
  std::vector<i32> ns;
  for (i32 i = 2; i < N; i++) {
    i32 k = s.kth(i);
    if (k > count) {
      break;
    }
    ns.clear();
    for (i32 j = k; j <= count; j += k) {
      ns.emplace_back(s.kth(j));
    }
    for (i32 n : ns) {
      s.remove(n);
      count -= 1;
    }
  }

  std::bitset<N> lucky;
  std::vector<i32> elems;
  for (i32 i = 1; i <= count; i++) {
    const i32 k = s.kth(i);
    lucky.set(k);
    elems.emplace_back(k);
  }

  for (i32 n; cin >> n;) {
    if (n % 2 != 0) {
      cout << n << " is not the sum of two luckies!" << '\n';
      continue;
    }

    i32 a = -1;
    i32 mid_at = std::upper_bound(elems.begin(), elems.end(), n / 2) - elems.begin();

    for (i32 i = mid_at - 1; i >= 0; i--) {
      if (lucky.test(n - elems[i])) {
        a = n - elems[i];
        break;
      }
    }

    if (a == -1) {
      cout << n << " is not the sum of two luckies!" << '\n';
    } else {
      cout << n << " is the sum of " << n - a << " and " << a << ".\n";
    }
  }

  return 0;
}
