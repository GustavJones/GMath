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
  DynamicArray(const T &_value) : m_values() { m_values.push_back(_value); }
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

  template<typename... Args>
  T &EmplaceBack(Args&& ... _args) { return m_values.emplace_back(_args...);}

  void Insert(const GMath::size_t _index, const T &_value) { m_values.insert(m_values.begin() + _index, _value); }

  void Erase(const GMath::size_t _index, const GMath::size_t _count = 1) { m_values.erase(m_values.begin() + _index, m_values.begin() + _index + _count); }

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
