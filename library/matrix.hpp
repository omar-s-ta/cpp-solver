#ifndef MATRIX
#define MATRIX

#include <algorithm>
#include <vector>

template <typename T>
struct matrix {
  template <typename O>
  using container = std::vector<std::vector<O>>;

  template <typename O>
  using enable_if_convertible = typename std::enable_if<std::is_convertible<O, T>::value>::type;

  container<T>::iterator begin() { return values.begin(); };
  container<T>::const_iterator begin() const { return values.begin(); }
  container<T>::iterator end() { return values.end(); }
  container<T>::const_iterator end() const { return values.end(); }

  static matrix<T> Identity(const int n) {
    matrix<T> identity(n);
    for (int i = 0; i < n; i++) identity[i][i] = 1;
    return identity;
  }

  matrix(const int _rows, const int _cols = -1) { init(_rows, _cols); }

  template <typename U, typename = enable_if_convertible<U>>
  matrix(const int _rows, const int _cols, U&& value) {
    init(_rows, _cols, std::forward<U>(value));
  }

  matrix(const std::vector<std::string>& _values) { init(_values); }

  template <typename U>
  explicit matrix(const container<U>& _values) {
    init(_values);
  }

  void init(const int _rows, const int _cols = -1) {
    rows = _rows;
    cols = _cols < 0 ? _rows : _cols;
    values.assign(rows, std::vector<T>(cols, 0));
  }

  template <typename U, typename = enable_if_convertible<U>>
  void init(const int _rows, const int _cols, U&& value) {
    rows = _rows;
    cols = _cols;
    values.assign(rows, std::vector<T>(cols, std::forward<U>(value)));
  }

  void init(const std::vector<std::string>& _values) {
    const int _rows = _values.size();
    const int _cols = _values[0].size();
    values = container<char>(_rows);
    for (int i = 0; i < _rows; i++) {
      values[i] = std::vector<char>(_values[i].begin(), _values[i].end());
    }
    rows = _rows;
    cols = _cols;
  }

  template <typename U>
  void init(const container<U>& _values) {
    values = _values;
    rows = values.size();
    cols = values[0].size();
  }

  template <typename U, typename = enable_if_convertible<U>>
  inline void fill(U&& value) {
    for (auto& row : values) std::fill(row.begin(), row.end(), std::forward<U>(value));
  }

  template <typename Fun>
  inline void for_each(Fun&& f) {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++) f(values[i][j]);
  }
  template <typename Fun>
  inline void for_each_with_indices(Fun&& f) {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++) f(i, j, values[i][j]);
  }
  template <typename Pred>
  inline bool for_all(Pred&& pred) const {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        if (!pred(values[i][j])) return false;
    return true;
  }
  template <typename Pred>
  inline bool for_all_with_indices(Pred&& pred) const {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        if (!pred(i, j, values[i][j])) return false;
    return true;
  }
  template <typename Pred>
  inline bool any_of(Pred&& pred) const {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        if (pred(values[i][j])) return true;
    return false;
  }
  template <typename Pred>
  inline bool any_of_with_indices(Pred&& pred) const {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        if (pred(i, j, values[i][j])) return true;
    return false;
  }

  template <typename U, typename = enable_if_convertible<U>>
  inline int count(U&& value) const {
    int cnt = 0;
    for (const auto& row : values)
      cnt += std::count(row.begin(), row.end(), std::forward<U>(value));
    return cnt;
  }
  template <typename Pred>
  inline int count_if(Pred&& pred) const {
    int cnt = 0;
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++) cnt += pred(values[i][j]);
    return cnt;
  }
  template <typename Pred>
  inline int count_if_with_indices(Pred&& pred) const {
    int cnt = 0;
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++) cnt += pred(i, j, values[i][j]);
    return cnt;
  }

  std::pair<int, int> find_first_of(const T& value) const {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        if (values[i][j] == value) return {i, j};
    return {-1, -1};
  }
  template <typename Pred>
  std::pair<int, int> find_first_if(Pred&& pred) const {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        if (pred(values[i][j])) return {i, j};
    return {-1, -1};
  }
  template <typename Pred>
  std::pair<int, int> find_first_if_with_indices(Pred&& pred) const {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        if (pred(i, j, values[i][j])) return {i, j};
    return {-1, -1};
  }

  inline int rows_count() const { return rows; }
  inline int cols_count() const { return cols; }
  inline bool is_square() const { return rows == cols; }

  inline bool contains_pos(const int i, const int j) const {
    assert(!values.empty());
    return 0 <= i && i < rows && 0 <= j && j < cols;
  }
  inline bool contains_pos(const std::pair<int, int>& p) const {
    return contains_pos(p.first, p.second);
  }
  inline bool contains_pos(const std::tuple<int, int>& t) const {
    return contains_pos(std::get<0>(t), std::get<1>(t));
  }

  inline bool position_on_edge(const int i, const int j) const {
    assert(!values.empty());
    return i == 0 || i == rows - 1 || j == 0 || j == cols - 1;
  }
  inline bool position_on_edge(const std::pair<int, int>& p) const {
    return position_on_edge(p.first, p.second);
  }
  inline bool position_on_edge(const std::tuple<int, int>& t) const {
    return position_on_edge(std::get<0>(t), std::get<1>(t));
  }

  void transpose() {
    if (is_square()) {
      for (int i = 0; i < rows; i++)
        for (int j = i + 1; j < cols; j++) std::swap(values[i][j], values[j][i]);
    } else {
      matrix<T> t(cols, rows);
      for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) t[j][i] = values[i][j];
      *this = t;
    }
  }

  inline void rotate(const bool clockwise = true) {
    if (clockwise)
      rotate_90_clockwise();
    else
      rotate_90_anti_clockwise();
  }

  void reflect_vertically() {
    const int mid = cols / 2;
    for (int i = 0; i < mid; i++)
      for (int j = 0; j < rows; j++) std::swap(values[j][i], values[j][rows - i - 1]);
  }

  void reflect_horizontally() { std::reverse(values.begin(), values.end()); }

  template <typename U>
  matrix<T>& operator+=(U&& u) {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++) values[i][j] += u;
    return *this;
  }
  template <typename U>
  matrix<T>& operator-=(U&& u) {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++) values[i][j] -= u;
    return *this;
  }
  template <typename U>
  matrix<T>& operator*=(U&& u) {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++) values[i][j] *= u;
    return *this;
  }
  template <typename U>
  matrix<T>& operator/=(U&& u) {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++) values[i][j] /= u;
    return *this;
  }
  template <typename U>
  matrix<T> operator+(U&& u) const {
    return matrix<T>(*this) += u;
  }
  template <typename U>
  matrix<T> operator-(U&& u) const {
    return matrix<T>(*this) -= u;
  }
  template <typename U>
  matrix<T> operator*(U&& u) const {
    return matrix<T>(*this) *= u;
  }
  template <typename U>
  matrix<T> operator/(U&& u) const {
    return matrix<T>(*this) /= u;
  }

  template <typename U>
  matrix<T>& operator+=(const matrix<U>& other) {
    assert(rows == other.rows && cols == other.cols);
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++) values[i][j] += other[i][j];
    return *this;
  }
  template <typename U>
  matrix<T>& operator-=(const matrix<U>& other) const {
    assert(rows == other.rows && cols == other.cols);
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++) values[i][j] -= other[i][j];
    return *this;
  }
  template <typename U>
  matrix<T>& operator/=(const matrix<U>& other) const {
    assert(rows == other.rows && cols == other.cols);
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++) values[i][j] /= other[i][j];
    return *this;
  }

  template <typename U>
  matrix<T> operator+(const matrix<U>& other) const {
    return matrix<T>(*this) += other;
  }
  template <typename U>
  matrix<T> operator-(const matrix<U>& other) const {
    return matrix<T>(*this) -= other;
  }
  template <typename U>
  matrix<T> operator/(const matrix<U>& other) const {
    return matrix<T>(*this) /= other;
  }

  template <typename U>
  matrix<T> operator*(const matrix<U>& other) const {
    assert(cols == other.cols);
    matrix<T> product(rows, other.cols);
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        if (values[i][j] != 0)
          for (int k = 0; k < other.cols; k++) product[i][k] += values[i][j] * other[j][k];
    return product;
  }
  template <typename U>
  matrix<T>& operator*=(const matrix<U>& other) {
    return *this = *this * other;
  }
  template <typename U>
  std::vector<T> operator*(const std::vector<U>& column) const {
    assert(cols == int(column.size()));
    std::vector<T> product(rows);
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++) product[i] += values[i][j] * column[j];
    return product;
  }

  matrix<T> power(int p) const {
    assert(p >= 0);
    assert(is_square());
    matrix<T> mat = *this;
    matrix<T> result = Identity(rows);
    while (p > 0) {
      if (p & 1) result *= mat;
      p >>= 1;
      if (p > 0) mat *= mat;
    }
    return result;
  }

  std::vector<T>& operator[](const int at) { return values[at]; }
  const std::vector<T>& operator[](const int at) const { return values[at]; }

  T& operator[](const std::pair<int, int>& at) { return values[at.first][at.second]; }
  const T& operator[](const std::pair<int, int>& at) const { return values[at.first][at.second]; }
  T& operator[](const std::tuple<int, int>& at) { return values[std::get<0>(at)][std::get<1>(at)]; }
  const T& operator[](const std::tuple<int, int>& at) const {
    return values[std::get<0>(at)][std::get<1>(at)];
  }

  template <typename U>
  inline bool operator==(const matrix<U>& other) const {
    return values == other.values;
  }

  inline void pretty_print(std::ostream& out) const {
    if (std::is_same<T, char>::value) {
      for (int i = 0; i < rows; i++) {
        out << '[';
        for (int j = 0; j < cols; j++) out << values[i][j];
        out << "]\n";
      }
    } else {
      for (int i = 0; i < rows; i++) {
        out << '[';
        for (int j = 0; j < cols; j++) {
          out << values[i][j];
          if (j < cols - 1) out << ", ";
        }
        out << "]\n";
      }
    }
  }

  inline void print(std::ostream& out) const {
    if (std::is_same<T, char>::value) {
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) out << values[i][j];
        out << '\n';
      }
    } else {
      for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) out << values[i][j] << " \n"[j == cols - 1];
    }
  }

  template <typename U>
  friend std::istream& operator>>(std::istream& in, matrix<U>& mat) {
    assert(!mat.values.empty());
    if constexpr (std::is_same<T, char>::value) {
      for (int i = 0; i < mat.rows; i++) {
        std::string chars;
        in >> chars;
        mat[i] = std::vector<char>(chars.begin(), chars.end());
      }
    } else {
      for (int i = 0; i < mat.rows; i++)
        for (int j = 0; j < mat.cols; j++) in >> mat[i][j];
    }
    return in;
  }

 private:
  void rotate_90_clockwise() {
    transpose();
    const int mid = cols / 2;
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < mid; j++) std::swap(values[i][j], values[i][cols - j - 1]);
  }

  void rotate_90_anti_clockwise() {
    transpose();
    const int mid = rows / 2;
    for (int i = 0; i < mid; i++) std::swap(values[i], values[rows - i - 1]);
  }

 private:
  std::vector<std::vector<T>> values;
  int rows, cols;
};

#endif
