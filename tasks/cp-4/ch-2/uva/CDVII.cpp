#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <ios>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

static constexpr const size_t H = 24;
static std::array<u32, H> tolls;

/*
 * Parsing the input is the most annoying part.
 * - Use a vector to sort the times of entry and exit of each car.
 * - Or use two min-heaps for entry and exit.
 */
class problem {
 public:
  void solve_one() {
    for (size_t i = 0; i < H; i++) {
      in >> tolls[i];
    }
    in.ignore();

    std::map<std::string, std::vector<entry>> photos;
    for (std::string description, plate, timestamp, state;
         std::getline(in, description) && !description.empty();) {
      std::istringstream iss(description);

      entry meta;
      char colon;
      iss >> plate;
      iss >> meta.month >> colon >> meta.day >> colon >> meta.hour >> colon >> meta.minute;
      iss >> state >> meta.kilo;

      meta.is_enter = state == "enter";

      photos[plate].emplace_back(meta);
    }

    for (auto& photo : photos) {
      std::vector<entry>& entries = photo.second;
      std::sort(entries.begin(), entries.end());

      i32 bill = 200;
      for (size_t i = 1; i < entries.size(); i++) {
        if (!entries[i - 1].is_enter || entries[i].is_enter) {
          continue;
        }
        bill += 100 + (entries[i] - entries[i - 1]);
      }
      if (bill != 200) {
        out << photo.first << " $" << std::fixed << std::setprecision(2) << bill / 100.0 << '\n';
      }
    }
  }

  void solve() {
    u32 nt;
    in >> nt;
    for (u32 _ = 1; _ <= nt; _++) {
      if (_ > 1) {
        out << '\n';
      }
      solve_one();
    }
  }

  problem(std::istream& in, std::ostream& out) : in(in), out(out) {}

 private:
  struct entry {
    u16 month, day, hour, minute;
    bool is_enter;
    i32 kilo;

    bool operator<(const entry& other) const {
      return std::tie(day, hour, minute) < std::tie(other.day, other.hour, other.minute);
    }

    i32 operator-(const entry& other) const {
      return std::abs(kilo - other.kilo) * tolls[other.hour];
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
