#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string>

inline char to_lower(const char ch) {
  return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}
inline bool is_alpha(const char ch) { return std::isalpha(static_cast<unsigned char>(ch)); }

/*
 * Utilize STL and stringstream for easier tokenization.
 */
class problem {
 public:
  void solve() {
    std::set<std::string> words;
    for (std::string line; std::getline(in, line);) {
      std::replace_if(line.begin(), line.end(), [](const char ch) { return !is_alpha(ch); }, ' ');
      std::transform(line.begin(), line.end(), line.begin(), to_lower);

      std::istringstream iss(line);
      std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
                std::inserter(words, words.end()));
    }

    for (const auto& word : words) {
      out << word << '\n';
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
