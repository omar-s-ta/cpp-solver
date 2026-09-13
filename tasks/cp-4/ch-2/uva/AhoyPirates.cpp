#include <bitset>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using namespace std;

const size_t N = 1024000 + 1;

bitset<N> a;

struct Node {
  u32 b, e, m;
  i32 value = 0;
  char cmd = ' ';

  void update(char new_cmd) {
    if (new_cmd == 'I') {
      value = e - b + 1 - value;
      switch (cmd) {
        case 'E':
          cmd = 'F';
          break;
        case 'F':
          cmd = 'E';
          break;
        case 'I':
          cmd = ' ';
          break;
        case ' ':
          cmd = 'I';
          break;
      }
    } else {
      value = new_cmd == 'F' ? e - b + 1 : 0;
      cmd = new_cmd;
    }
  }

  void update(const Node& lft, const Node& rgt) { value = lft.value + rgt.value; }
  void accumulate(const Node& other) { value += other.value; }
};

class SegmentTree {
 public:
  SegmentTree(size_t n) { tree = std::vector<Node>(n << 2); }

  void init(u32 b, u32 e) { init(0, b, e); }

  void update(u32 b, u32 e, char cmd) { update(0, b, e, cmd); }

  Node query(u32 b, u32 e) { return query(0, b, e); }

 private:
  inline u32 L(u32 i) const { return 1 + (i << 1); }
  inline u32 R(u32 i) const { return 2 + (i << 1); }

  void push_down(u32 root) {
    Node& node = tree[root];
    if (node.cmd != ' ') {
      tree[L(root)].update(node.cmd);
      tree[R(root)].update(node.cmd);
      node.cmd = ' ';
    }
  }

  void init(u32 root, u32 b, u32 e) {
    Node& node = tree[root];
    node.b = b;
    node.e = e;
    node.m = b + ((e - b) >> 1);
    node.cmd = ' ';

    if (b == e) {
      tree[root].value = a[b];
    } else {
      init(L(root), b, node.m);
      init(R(root), node.m + 1, e);
      tree[root].update(tree[L(root)], tree[R(root)]);
    }
  }

  void update(u32 root, u32 b, u32 e, char cmd) {
    Node& node = tree[root];
    if (node.b >= b && node.e <= e) {
      node.update(cmd);
    } else {
      push_down(root);
      if (b <= node.m) {
        update(L(root), b, min(node.m, e), cmd);
      }
      if (e > node.m) {
        update(R(root), max(b, node.m + 1), e, cmd);
      }
      node.update(tree[L(root)], tree[R(root)]);
    }
  }

  Node query(u32 root, u32 b, u32 e) {
    Node& node = tree[root];
    if (node.b >= b && node.e <= e) {
      return node;
    } else {
      push_down(root);
      Node result;
      if (b <= node.m) {
        result.accumulate(query(L(root), b, min(node.m, e)));
      }
      if (e > node.m) {
        result.accumulate(query(R(root), max(b, node.m + 1), e));
      }
      return result;
    }
  }

  std::vector<Node> tree;
};

SegmentTree tree(N);

i32 main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  u32 t;
  cin >> t;

  for (u32 ti = 1; ti <= t; ti++) {
    a.reset();

    u32 m, pn, at = 0;
    cin >> m;

    for (string pirates; m > 0; m--) {
      cin >> pn >> pirates;
      for (; pn > 0; pn--) {
        for (char ch : pirates) {
          a[at++] = ch == '1';
        }
      }
    }

    cout << "Case " << ti << ":\n";

    u32 qs, a, b;
    cin >> qs;

    tree.init(0, at - 1);
    for (u32 qi = 1; qs > 0; qs--) {
      char cmd;
      cin >> cmd >> a >> b;
      if (cmd == 'S') {
        cout << 'Q' << qi++ << ": " << tree.query(a, b).value << '\n';
      } else {
        tree.update(a, b, cmd);
      }
    }
  }

  return 0;
}
