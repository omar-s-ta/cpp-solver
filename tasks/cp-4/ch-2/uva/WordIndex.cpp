#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class problem {
 public:
  void solve() {
    init_no_allocation();

    for (std::string s; in >> s;) {
      out << id[s] << '\n';
    }
  }

  problem(std::istream& in, std::ostream& out) : in(in), out(out) {}

 private:
  void init_extra_allocation() {
    std::vector<std::string> words;

    words.reserve(MAX);
    id.reserve(MAX);

    for (char ch = 'a'; ch <= 'z'; ch++) {
      words.emplace_back(std::string(1, ch));
    }
    for (size_t i = 0; i < words.size(); i++) {
      const std::string& word = words[i];
      if (word.size() == 5 || word.back() == 'z') {
        continue;
      }
      for (char ch = word.back() + 1; ch <= 'z'; ch++) {
        words.emplace_back(word + ch);
      }
    }

    for (size_t i = 0; i < words.size(); i++) {
      id[words[i]] = i + 1;
    }
  }

  void init_no_allocation() {
    id.reserve(MAX);

    uint32_t count = 1;
    for (uint32_t len = 1; len <= 5; len++) {
      std::string s(len, 'a');
      for (int i = 0; i < len; i++) {
        s[i] = 'a' + i;
      }

      while (true) {
        id[s] = count++;

        // position i in a length-L combination
        // has a fixed maximum letter: 'z' - (L - 1 - i)
        int at = len - 1;
        while (at >= 0 && s[at] == 'z' - (len - 1 - at)) {
          at -= 1;
        }

        if (at < 0) {
          break;
        }

        s[at] += 1;
        for (int i = at + 1; i < len; i++) {
          s[i] = s[i - 1] + 1;
        }
      }
    }
  }

  std::istream& in;
  std::ostream& out;

  std::unordered_map<std::string, uint32_t> id;

  static constexpr const size_t MAX = 83681;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  problem solver(std::cin, std::cout);
  solver.solve();
  return 0;
}
