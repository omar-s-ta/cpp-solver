#ifndef CHAR_UTILS
#define CHAR_UTILS

#include <cctype>
const std::size_t ALPHABETS_SIZE = 26;

const std::string LOWER_ALPHABETS = "abcdefghijklmnopqrstuvwxyz";
const std::string UPPER_ALPHABETS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string VOWELS = "aeiou";

inline char to_lower(const char ch) {
  return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}
inline char to_upper(const char ch) {
  return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
}
inline bool is_upper(const char ch) { return std::isupper(static_cast<unsigned char>(ch)); }
inline bool is_alpha(const char ch) { return std::isalpha(static_cast<unsigned char>(ch)); }
inline bool is_lower(const char ch) { return std::islower(static_cast<unsigned char>(ch)); }
inline bool is_digit(const char ch) { return std::isdigit(static_cast<unsigned char>(ch)); }
inline bool is_space(const char ch) { return std::isspace(static_cast<unsigned char>(ch)); }
inline bool is_punct(const char ch) { return std::ispunct(static_cast<unsigned char>(ch)); }
inline bool is_vowel(const char ch) { return VOWELS.find(to_lower(ch)) != std::string::npos; }
inline bool is_consonant(const char ch) { return !is_vowel(ch); }

#endif
