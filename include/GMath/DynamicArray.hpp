#pragma once
#include "GMath/Types.hpp"
#include <initializer_list>
#include <vector>

namespace GMath {
template <typename T> class DynamicArray {
private:
  std::vector<T> m_values;

public:
  DynamicArray() = default;
  explicit DynamicArray(const GMath::size_t _n) : m_values(_n) {};
  DynamicArray(const std::initializer_list<T> &_list) : m_values(_list.begin(), _list.end()) {};
  DynamicArray(DynamicArray &&) = default;
  DynamicArray(const DynamicArray &) = default;
  DynamicArray &operator=(DynamicArray &&) = default;
  DynamicArray &operator=(const DynamicArray &) = default;
  ~DynamicArray() = default;

  GMath::size_t Size() const { return m_values.size(); };
  void Resize(const GMath::size_t _size) { m_values.resize(_size); };
  void Clear() { m_values.clear(); };
  void PushBack(const T &_val) { m_values.push_back(_val); }
  void PushBack(T &&_val) { m_values.push_back(_val); }

  [[nodiscard]]
  const T &operator[](const GMath::size_t _n) const {
    return m_values[_n];
  };

  [[nodiscard]]
  T &operator[](const GMath::size_t _n) {
    return m_values[_n];
  };
};
} // namespace GMath
