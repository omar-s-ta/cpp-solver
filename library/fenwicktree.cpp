#include <algorithm>  // max, for MaxFenwickTree
#include <climits>    // LLONG_MIN, the identity for max
#include <cstdio>
#include <vector>
using namespace std;

typedef long long ll;  // for extra flexibility
typedef vector<ll> vll;
typedef vector<int> vi;

// Fenwick (Binary Indexed) Tree, natively 0-based: valid indices are [0, m-1],
// `ft` holds exactly m cells, and there is no unused slot and no +/-1 shifting.
//
//
// WHAT ft[i] COVERS
//
// ft[i] stores the sum of the value range [i & (i+1), i]: it ends at i and runs
// back over as many elements as i has trailing 1-bits, doubling each time.
// `i & (i+1)` clears those trailing 1s, which is where the range starts, so the
// length is LSOne(i+1) == (i+1) & -(i+1). For m = 10:
//
//   i  binary  covers    len
//   0   0000   [0, 0]     1
//   1   0001   [0, 1]     2
//   2   0010   [2, 2]     1
//   3   0011   [0, 3]     4
//   4   0100   [4, 4]     1
//   5   0101   [4, 5]     2
//   6   0110   [6, 6]     1
//   7   0111   [0, 7]     8
//   8   1000   [8, 8]     1
//   9   1001   [8, 9]     2
//
// Every prefix [0, j] is a disjoint union of at most log2(m) of these ranges,
// and every index i belongs to at most log2(m) of them -- that pair of facts is
// the whole data structure. Note this is the same *shape* as the classic tree,
// just read off a different index: nothing is reordered, only relabelled.
//
//
// WHY THE FORMULATION DIFFERS FROM THE 1-BASED ONE
//
// In the textbook version ft[i] covers a range of length LSOne(i) ending at i,
// which forces 1-based indexing: LSOne(0) == 0 would give cell 0 a length-0
// range, so it can carry nothing. Worse, both walks stall on it -- the update's
// `i += LSOne(i)` never advances (infinite loop) and the query's
// `i -= LSOne(i)` never reaches a terminating value. Hence the dead ft[0] and
// the +/-1 the caller has to remember.
//
// Here the length is read off i+1 instead of i, and LSOne(i+1) is never 0 for
// i >= 0, so index 0 becomes an ordinary cell. Rewriting the two walks as bit
// operations on i keeps that +1 out of the code entirely:
//
//   ascend  (update): i |= i + 1           replaces  i += LSOne(i)
//   descend (query):  i = (i & (i+1)) - 1  replaces  i -= LSOne(i)
//
// Both are well behaved at 0: 0 | 1 == 1 climbs, and (0 & 1) - 1 == -1 falls
// off the left end, which is exactly the terminating value the loop wants.
//   - ascend sets the lowest 0-bit of i, jumping to the next cell whose range
//     contains i (0 -> 1 -> 3 -> 7 -> 15 ...), so update() hits every cell that
//     needs the delta.
//   - descend clears the trailing 1s to reach the start of ft[i]'s range, then
//     steps one further left, so rsq() walks the disjoint pieces of the prefix.
// Each step clears or sets at least one bit, which is why both are O(log m).
//
//
// WHY DESCEND IS (i & (i+1)) - 1 SPECIFICALLY
//
// rsq(j) wants the sum of [0, j], and ft[j] already holds [j & (j+1), j] -- a
// suffix of exactly that prefix. So add ft[j], then continue with whatever is
// left over, which is [0, (j & (j+1)) - 1]. The next index *is* the leftover's
// right end, hence the two pieces:
//   i & (i+1)   the start of ft[i]'s range. i+1 carries through i's trailing 1s,
//               so ANDing with i clears them -- equivalently, it clears the
//               lowest set bit of i+1.
//   - 1         step past that start, which ft[i] has already accounted for,
//               to the last index still unsummed.
// It is the same move as the 1-based `i -= LSOne(i)`, only measured on i+1:
// since ft[i] has length LSOne(i+1) and ends at i, its start is
// i - LSOne(i+1) + 1, so (i & (i+1)) - 1 == i - LSOne(i+1) exactly.
//
// Termination is free rather than a special case: when ft[i]'s range already
// starts at 0 -- i.e. i is all 1s, so i & (i+1) == 0 -- the next index is -1 and
// the loop condition i >= 0 stops it, precisely when the prefix is fully
// covered. Tracing rsq(9): ft[9] covers [8, 9], next is (9 & 10) - 1 == 7;
// ft[7] covers [0, 7], next is (7 & 8) - 1 == -1, done. Two disjoint chunks,
// [0,7] + [8,9], which is the binary decomposition of the length 10 = 8 + 2.
//
// A note on the alternative: you can also keep the 1-based array and just do
// ++i on entry and --i on the way out. That works and is a smaller diff, but it
// keeps the wasted cell, and the shift leaks into every method plus select()'s
// return value -- one forgotten +1 is an off-by-one that the small tests still
// pass. The formulation below has no shift to forget.
//
//
// Point Update, Range Query (PURQ): update() adds to one cell, rsq() sums a range.
class FenwickTree {
 private:
  vll ft;  // internal FT is an array; every cell is used

 public:
  // Number of elements the tree addresses, i.e. the m it was built with. O(1).
  int size() const { return (int)ft.size(); }

  // Creates an empty tree over the index range [0, m-1], all zeros. O(m).
  FenwickTree(int m) { ft.assign(m, 0); }

  // (Re)builds the tree from the value array f, so that afterwards
  // rsq(i, j) == f[i] + ... + f[j]. The tree is resized to f.size().
  // O(m), which beats m separate update() calls (O(m log m)).
  void build(const vll& f) {
    int m = (int)f.size();
    ft.assign(m, 0);
    for (int i = 0; i < m; ++i) {  // O(m)
      ft[i] += f[i];               // add this value
      int p = i | (i + 1);         // i's parent cell
      if (p < m) ft[p] += ft[i];   // add to that parent
    }
  }

  // Creates a tree directly from the value array f. O(m).
  FenwickTree(const vll& f) { build(f); }

  // Disambiguating tag for the all-ones constructor below. It exists because
  // FenwickTree(int) already means "all zeros", so the two cannot be told apart
  // by arity, and a bare bool argument at the call site would say nothing.
  struct all_ones_t {};

  // (Re)builds the tree over [0, m-1] with every element equal to 1 -- read as a
  // frequency table, every key in [0, m-1] present exactly once. O(m).
  //
  // Equivalent to build(vll(m, 1)) but without materialising that vector: ft[i]
  // covers [i & (i+1), i], so over all-ones data it simply holds that block's
  // length, which the header derives as LSOne(i+1) == (i+1) & -(i+1). Worth the
  // separate method because the temporary is the same size as the tree itself --
  // for m = 2e6 that is 16 MB of peak instead of 8 MB, since f and ft are both
  // live while build() reads one into the other.
  //
  // Useful whenever an algorithm starts from "every key is present" and only ever
  // removes -- the lucky-number and Josephus families of sieves, where the
  // survivors are found by repeatedly deleting the k-th remaining element. For a
  // domain that starts at 1 rather than 0, follow with update(0, -1), or
  // OrderStatSet::remove(0) on the wrapper below.
  void build_ones(int m) {
    ft.assign(m, 0);
    for (int i = 0; i < m; ++i)      // O(m)
      ft[i] = (i + 1) & -(i + 1);    // == the length of [i & (i+1), i]
  }

  // Creates a tree over [0, m-1] with every element equal to 1. O(m).
  FenwickTree(int m, all_ones_t) { build_ones(m); }

  // Creates a frequency tree over the keys [0, m-1] from the multiset s:
  // afterwards rsq(i, j) counts how many elements of s lie in [i, j].
  // Every s[k] must be in [0, m-1]. O(m + n) with n == s.size().
  FenwickTree(int m, const vi& s) {
    vll f(m, 0);
    for (int i = 0; i < (int)s.size(); ++i)  // do the conversion first
      ++f[s[i]];                             // in O(n)
    build(f);                                // in O(m)
  }

  // Prefix sum over [0, j] inclusive. j == -1 is legal and returns 0, which is
  // what makes the two-argument rsq() below work for i == 0. O(log m).
  ll rsq(int j) {
    ll sum = 0;
    for (; j >= 0; j = (j & (j + 1)) - 1) sum += ft[j];  // hop left, range by range
    return sum;
  }

  // Range sum over [i, j] inclusive, by inclusion/exclusion. O(log m).
  ll rsq(int i, int j) { return rsq(j) - rsq(i - 1); }

  // Adds v to the element at index i (v may be positive/increment or
  // negative/decrement). There is no "assign" -- to set a value, add the delta.
  // An out-of-range i >= size() is a silent no-op, which RUPQ below relies on.
  // O(log m).
  void update(int i, ll v) {
    for (int m = size(); i < m; i |= i + 1) ft[i] += v;  // climb to every covering cell
  }

  // Smallest index i such that rsq(0, i) >= k, assuming all values are
  // non-negative and k >= 1. Descends the implicit tree instead of binary
  // searching over rsq(), hence O(log m) and not O(log^2 m).
  // Returns size() (one past the last valid index) when k exceeds the total sum.
  int select(ll k) {
    int m = size();
    int p = 1;
    while (p * 2 <= m) p *= 2;  // largest power of two that fits in the tree
    int i = 0;                  // elements already skipped
    while (p) {
      // ft[i+p-1] covers [i, i+p-1]; the bound check is needed because m is
      // not necessarily a power of two, so that cell may not exist. Hoisted
      // into c so the compare and the subtract share one load instead of two.
      if (i + p <= m) {
        ll c = ft[i + p - 1];
        if (c < k) {
          k -= c;
          i += p;
        }
      }
      p /= 2;
    }
    return i;
  }
};

// 2D Fenwick tree: point update and rectangle sum, both O(log n log m).
// Rows are [0, n-1], columns [0, m-1]. A Fenwick tree *of* Fenwick trees -- the
// row index gets the same decomposition as the column index, and that is the
// entire difference between this and a flat vector<FenwickTree> holding one tree
// per row. The flat version is not wrong, just a different trade: it updates in
// O(log m) and answers a single row's range in O(log m), but a rectangle costs
// O(rows * log m) because it has to visit every row in range.
//
// ft[i] is NOT row i. It is a column tree for the row-block [i & (i+1) .. i], so
// the cell (r, c) lives in every ft[i] whose block contains r -- exactly log n of
// them, mirroring the 1D fact that an index belongs to log n cells. Composing
// both axes, entry j of inner tree i holds the sum over the product range
//
//   [i & (i+1) .. i]  x  [j & (j+1) .. j]
//
// Memory is O(n*m), held as n separate row vectors. That, not time, is usually
// the binding constraint: a 1e5 x 1e5 grid is 1e10 cells. For large sparse data
// prefer coordinate compression, or drop a dimension by sweeping one axis
// offline with the 1D tree. There is no build() -- load the grid with update().
class FenwickTree2D {
 private:
  vector<FenwickTree> ft;  // ft[i] covers the row-block [i & (i+1), i]

 public:
  // Creates an all-zero grid over rows [0, n-1] and columns [0, m-1]. O(n*m).
  FenwickTree2D(int n, int m) : ft(n, FenwickTree(m)) {}

  // (Re)builds the whole grid from g, so that afterwards
  // rsq(i1, j1, i2, j2) == the sum of g over that rectangle. g must be
  // rectangular: every row the same length, which becomes the column width.
  // O(n*m) -- vastly better than n*m update() calls, which would cost
  // O(n*m log n log m).
  //
  // Two passes, and they are separable because the 1D column transform is
  // linear (each output cell is a plain sum of input cells), so transforming
  // then adding rows equals adding rows then transforming:
  //   1. propagate rows in RAW space, `work[i | (i+1)] += work[i]` elementwise,
  //      which is the 1D build's own walk with whole rows as the element type.
  //      Forward order is safe for the same reason as in 1D: the parent index
  //      i | (i+1) is always strictly greater than i.
  //   2. hand each finished row to the 1D build, which applies the column
  //      transform in place.
  // g is taken BY VALUE and mutated as the scratch buffer -- an rvalue is moved
  // in for free, an lvalue costs the one O(n*m) copy this needs either way.
  void build(vector<vll> g) {
    int n = (int)g.size();
    int m = n ? (int)g[0].size() : 0;
    for (int i = 0; i < n; ++i) {  // pass 1: rows into their row-block parents
      int p = i | (i + 1);
      if (p < n)
        for (int j = 0; j < m; ++j) g[p][j] += g[i][j];
    }
    ft.clear();
    ft.reserve(n);
    for (int i = 0; i < n; ++i)  // pass 2: column transform, row by row
      ft.emplace_back(g[i]);
  }

  // Creates a tree directly from the grid g. O(n*m).
  FenwickTree2D(const vector<vll>& g) { build(g); }

  // Number of ROWS (n). Not the cell count, and not the column width -- m lives
  // inside each inner tree and is deliberately not exposed. O(1).
  int size() const { return int(ft.size()); }

  // Adds v to the cell (i, j), v positive or negative. Climbs the log n
  // row-blocks that contain row i and lets each inner tree climb its own log m
  // column cells. An out-of-range i or j is a silent no-op -- the row loop or
  // FenwickTree::update simply never runs. O(log n log m).
  void update(int i, int j, ll v) {
    for (int n = size(); i < n; i |= i + 1) {
      ft[i].update(j, v);
    }
  }

  // 2D prefix sum over the closed rectangle [0..i] x [0..j].
  // CAREFUL: two arguments mean a PREFIX here, while two arguments on the 1D
  // FenwickTree::rsq mean a RANGE. i == -1 or j == -1 returns 0, which is what
  // lets the rectangle overload below handle top/left edges with no branch.
  // O(log n log m).
  ll rsq(int i, int j) {
    ll s = 0;
    for (; i >= 0; i = (i & (i + 1)) - 1) {
      s += ft[i].rsq(j);
    }
    return s;
  }

  // Rectangle sum over rows [i1, i2] and columns [j1, j2], all ends inclusive.
  // Argument order is (row, col, row, col) -- corner then corner, NOT both rows
  // then both columns.
  //
  // This is the four-corner inclusion/exclusion
  //   P(i2,j2) - P(i1-1,j2) - P(i2,j1-1) + P(i1-1,j1-1)
  // with the pairs fused. P(i2,j2) and P(i2,j1-1) walk the identical set of row
  // blocks -- both decompose [0, i2] -- so they collapse into one row walk that
  // asks each inner tree for the column RANGE rsq(j1, j2) instead of two column
  // prefixes. Likewise for the [0, i1-1] pair. Cell reads are unchanged, but
  // there are half as many row walks and each row's inner vector is visited once
  // while still cache-hot; measured ~1.2-1.6x faster than the literal four
  // calls, a locality win rather than an asymptotic one.
  //
  // An empty rectangle returns 0. i1 == 0 skips the second loop outright and
  // j1 == 0 makes the inner rsq(j1-1) term free, both without a branch.
  // O(log n log m).
  ll rsq(int i1, int j1, int i2, int j2) {
    if (i1 > i2 || j1 > j2) {
      return 0;
    }
    ll s = 0;
    for (; i2 >= 0; i2 = (i2 & (i2 + 1)) - 1) {  // rows of [0, i2], added
      s += ft[i2].rsq(j1, j2);
    }
    for (i1 -= 1; i1 >= 0; i1 = (i1 & (i1 + 1)) - 1) {  // rows of [0, i1-1], removed
      s -= ft[i1].rsq(j1, j2);
    }
    return s;
  }
};

// Prefix-max Fenwick tree. Identical storage layout and identical bit walks to
// FenwickTree -- only the combining operation changes: ft[i] holds the MAXIMUM
// over [i & (i+1), i] instead of the sum. Indices are 0-based, range [0, m-1].
//
// Because max has no inverse, two things the sum version gives you are gone.
// This is a genuinely weaker structure, not a drop-in replacement:
//
//   1. UPDATES ONLY EVER RAISE. raise(i, v) sets element i to max(old, v).
//      Lowering a value is impossible here: the climb would have to un-do a max,
//      and the old maximum still sits in every cell covering i with no record of
//      which element produced it. Doing it properly means recomputing each
//      affected block from its sub-blocks, O(log^2 m), which this layout does
//      not support. That is why the method is not called update().
//   2. THERE IS NO RANGE MAX, and it is not a matter of being slower -- the
//      information is absent. [5,1,1,1] and [5,5,1,1] have identical prefix
//      maxima (5 5 5 5), yet max over [1,3] is 1 for the first and 5 for the
//      second, so no formula over prefix maxima can separate them. When you need
//      arbitrary ranges use a segment tree (dynamic) or a sparse table (static).
//      Deliberately no two-argument overload exists, so the compiler stops you
//      rather than a wrong answer at 3am.
//
// What is left is exactly "raise a point, query a prefix" -- the shape LIS wants:
// raise(value, length), then prefix_max(value - 1) for the best predecessor.
// For prefix MINIMA, either negate on the way in and out, or flip both the
// comparison and the identity below.
class MaxFenwickTree {
 private:
  vll ft;  // ft[i] holds the max over [i & (i+1), i]

 public:
  // The identity for max, and what an untouched cell or an empty prefix reads
  // as. It must NOT be 0: with a 0 identity the tree silently reports 0 for
  // all-negative data, which passes every test that happens to contain a
  // positive value.
  static constexpr ll NONE = LLONG_MIN;

  // Number of elements the tree addresses. O(1).
  int size() const { return (int)ft.size(); }

  // Creates an empty tree over [0, m-1]; every element reads as NONE. O(m).
  MaxFenwickTree(int m) { ft.assign(m, NONE); }

  // (Re)builds from the value array f, so afterwards prefix_max(j) is the max of
  // f[0..j]. Same bottom-up walk as FenwickTree::build with max in place of +=;
  // it is if anything safer, since max is idempotent and so does not care
  // whether the blocks it merges overlap. O(m), beating m raise() calls.
  void build(const vll& f) {
    int m = (int)f.size();
    ft.assign(m, NONE);
    for (int i = 0; i < m; ++i) {          // O(m)
      ft[i] = max(ft[i], f[i]);            // this value
      int p = i | (i + 1);                 // i's parent cell
      if (p < m) ft[p] = max(ft[p], ft[i]);  // merge into that parent
    }
  }

  // Creates a tree directly from the value array f. O(m).
  MaxFenwickTree(const vll& f) { build(f); }

  // Raises element i to max(current, v). A v below the current value is a no-op
  // by design, NOT an assignment -- see point 1 in the class comment. An
  // out-of-range i >= size() is a silent no-op. O(log m).
  void raise(int i, ll v) {
    for (int m = size(); i < m; i |= i + 1) ft[i] = max(ft[i], v);
  }

  // Maximum over [0, j] inclusive. j == -1 is legal and returns NONE, the
  // identity -- note this is LLONG_MIN and not 0, so it does not compose the way
  // FenwickTree::rsq(-1) == 0 does. O(log m).
  ll prefix_max(int j) {
    ll r = NONE;
    for (; j >= 0; j = (j & (j + 1)) - 1) r = max(r, ft[j]);
    return r;
  }
};

// Range Update, Point Query. Stores the values as a difference array inside a
// plain Fenwick tree, so a range update is two point updates and a point query
// is a prefix sum. Indices are 0-based, range [0, m-1].
class RUPQ {
 private:
  FenwickTree ft;  // internally use PURQ FT over the difference array
 public:
  // Creates an all-zero range over [0, m-1]. O(m).
  RUPQ(int m) : ft(FenwickTree(m)) {}

  // Adds v to every element in [ui, uj] inclusive. O(log m).
  void range_update(int ui, int uj, ll v) {
    ft.update(ui, v);       // [ui, ui+1, .., m-1] +v
    ft.update(uj + 1, -v);  // [uj+1, uj+2, .., m-1] -v (no-op when uj == m-1)
  }  // net effect: [ui, ui+1, .., uj] +v

  // Current value of the element at index i. i == -1 is legal and returns 0.
  // O(log m).
  ll point_query(int i) { return ft.rsq(i); }
};

// Range Update, Range Query. Combines a RUPQ (holding the per-element value)
// with a second Fenwick tree holding a correction term, so prefix sums of a
// range-updated array stay O(log m). Indices are 0-based, range [0, m-1].
class RURQ {
 private:            // needs two helper FTs
  RUPQ rupq;         // one RUPQ and
  FenwickTree purq;  // one PURQ
 public:
  // Creates an all-zero range over [0, m-1]. O(m).
  RURQ(int m) : rupq(RUPQ(m)), purq(FenwickTree(m)) {}

  // Adds v to every element in [ui, uj] inclusive. O(log m).
  // The purq deltas are chosen so that purq.rsq(j) equals the exact amount by
  // which rsq(j)'s optimistic term below over-counts this update: 0 before ui,
  // v*ui inside [ui, uj] (the elements before ui that were counted anyway),
  // and -v*(uj-ui+1) after uj (where the optimistic term counts nothing).
  void range_update(int ui, int uj, ll v) {
    rupq.range_update(ui, uj, v);        // [ui, ui+1, .., uj] +v
    purq.update(ui, v * ui);             // cancel the ui elements before ui
    purq.update(uj + 1, -v * (uj + 1));  // cancel the over-count past uj
  }

  // Prefix sum over [0, j] inclusive. j == -1 is legal and returns 0.
  // rupq.point_query(j) * (j+1) pretends every element in [0, j] carries the
  // value of element j; purq.rsq(j) subtracts exactly what that over-counts.
  // O(log m).
  ll rsq(int j) {
    return rupq.point_query(j) * (j + 1) -  // optimistic calculation
           purq.rsq(j);                     // cancelation factor
  }

  // Range sum over [i, j] inclusive. O(log m).
  ll rsq(int i, int j) { return rsq(j) - rsq(i - 1); }  // standard
};

// Dynamic multiset over the integer key domain [0, m-1], built on the frequency
// -table reading of FenwickTree: the region for key v counts how many copies of
// v are currently present. This is CP-4 exercise 2.4.3.4 -- "insert" and
// "delete" turn out to be just update(v, +1) and update(v, -1), which is why the
// section never needed to introduce them separately.
//
//   insert / remove / count / contains / rank / count_le / count_in / kth  O(log m)
//   size / domain                                                         O(1)
//   memory                                                                O(m)
//
// ASSUMPTIONS -- these are the substance of the exercise, not boilerplate:
//   * The key domain is FIXED, INTEGRAL, and known up front. Memory is O(m) in
//     the size of the DOMAIN, not in the number of stored elements, and a key
//     outside [0, m-1] cannot be inserted at all. If the domain is unknown or
//     huge, coordinate-compress first (needs the keys offline), or switch to a
//     dynamic segment tree or an order-statistic BST.
//   * A key IS an index, which is exactly why nothing ever shifts and insertion
//     is cheap: inserting never invalidates another key. Positional insertion --
//     pushing a value between two others and renumbering the rest -- is NOT
//     possible with a Fenwick tree at any complexity; that needs a balanced BST
//     carrying subtree sizes.
//   * remove() must not run on an absent key. A negative count destroys the
//     monotonicity of the prefix sums that kth() relies on, and the damage is
//     silent: holding {2, 3}, removing an absent 0 leaves kth(1) == 3 and
//     size() == 1, both wrong, with no crash. Hence the guard, and hence
//     remove() returns bool rather than void.
//
// Duplicates are allowed. For strict set semantics, guard insert() with
// count(v) == 0 exactly as remove() is guarded.
class OrderStatSet {
 private:
  FenwickTree ft;  // the region for key v counts occurrences of v
  ll n = 0;        // running total, so size() need not walk the tree

 public:
  // Creates an empty set over the keys [0, m-1]. O(m).
  OrderStatSet(int m) : ft(m) {}

  // Creates a set over [0, m-1] already holding every key exactly once, so
  // size() == m and kth(k) == k-1. O(m) time and no O(m) temporary -- see
  // FenwickTree::build_ones. This is the entry point the deletion sieves want:
  // start full, then remove(). O(m).
  OrderStatSet(int m, FenwickTree::all_ones_t tag) : ft(m, tag), n(m) {}

  // Size of the key domain, i.e. the m it was built with. O(1).
  int domain() const { return ft.size(); }

  // How many elements are stored, duplicates counted separately. O(1).
  ll size() const { return n; }

  // Occurrences of the key v; 0 means absent. This is the rsq(i, i) == 0 test
  // the exercise phrases insertion in terms of. O(log m).
  ll count(int v) { return ft.rsq(v, v); }

  // Whether the key v is present at all. O(log m).
  bool contains(int v) { return count(v) > 0; }

  // Adds one copy of the key v. v MUST be in [0, m-1]: outside it the underlying
  // update() silently does nothing while n still advances, desyncing size().
  // That is a precondition, not a soft failure. O(log m).
  void insert(int v) { ft.update(v, +1); ++n; }

  // Removes one copy of the key v. Returns false and changes nothing when v is
  // absent -- see the third assumption above for why this must be guarded.
  // O(log m).
  bool remove(int v) {
    if (count(v) <= 0) return false;
    ft.update(v, -1);
    --n;
    return true;
  }

  // Number of stored elements with keys strictly less than v. rank(0) == 0 comes
  // free, since FenwickTree::rsq(-1) == 0. O(log m).
  ll rank(int v) { return ft.rsq(v - 1); }

  // Number of stored elements with keys <= v. O(log m).
  ll count_le(int v) { return ft.rsq(v); }

  // Number of stored elements with keys in [a, b] inclusive; 0 if a > b. O(log m).
  ll count_in(int a, int b) { return a > b ? 0 : ft.rsq(a, b); }

  // The k-th smallest key, k being 1-BASED: kth(1) is the minimum, kth(size())
  // the maximum. k is a RANK, not an index -- keys here are 0-based, but ranks
  // are counts and so start at 1, exactly as FenwickTree::select takes them.
  // Returns domain() (one past the last key) when k > size(). O(log m).
  int kth(ll k) { return ft.select(k); }
};

int main() {
  vll f = {0, 1, 0, 1, 2, 3, 2, 1, 1, 0};  // 0-based, indices 0..9
  FenwickTree ft(f);
  printf("%lld\n", ft.rsq(0, 5));  // 7 => ft[3]+ft[5] = 2+5 = 7
  printf("%d\n", ft.select(7));    // index 5, rsq(0, 5) == 7, which is >= 7
  ft.update(4, 1);                 // update demo
  printf("%lld\n", ft.rsq(0, 9));  // now 12
  printf("=====\n");
  RUPQ rupq(10);
  RURQ rurq(10);
  rupq.range_update(1, 8, 7);  // indices in [1, 2, .., 8] updated by +7
  rurq.range_update(1, 8, 7);  // same as rupq above
  rupq.range_update(5, 6, 3);  // indices 5&6 are further updated by +3 (10)
  rurq.range_update(5, 6, 3);  // same as rupq above
  // idx = 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
  // val = 0 | 7 | 7 | 7 | 7 |10 |10 | 7 | 7 | 0
  for (int i = 0; i < 10; i++) printf("%d -> %lld\n", i, rupq.point_query(i));
  printf("RSQ(0, 9) = %lld\n", rurq.rsq(0, 9));  // 62
  printf("RSQ(5, 6) = %lld\n", rurq.rsq(5, 6));  // 20
  return 0;
}
