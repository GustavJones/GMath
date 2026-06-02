#pragma once
#include "GMath/Types.hpp"
#include <initializer_list>
#include <vector>
#include <iostream>

namespace GMath {
/*
 * An common interface for a dynamic array.
 */
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

  /*
   * Get the amount of items in the array.
   */
  [[nodiscard]]
  GMath::size_t Size() const { return m_values.size(); };

  /*
   * Set the amount of items in the array.
   */
  void Resize(const GMath::size_t _size) { m_values.resize(_size); };

  /*
   * Reset the array to empty.
   */
  void Clear() { m_values.clear(); };

  /*
   * Copy an object to the back of the array.
   */
  void PushBack(const T &_val) { m_values.push_back(_val); }

  /*
   * Move an object to the back of the array.
   */
  void PushBack(T &&_val) { m_values.push_back(std::move(_val)); }

  /*
   * Create an object at the back of the array.
   */
  template <typename... Args> T &EmplaceBack(Args &&..._args) {
    return m_values.emplace_back(std::move(_args...));
  }

  /*
   * Inserts a copy of an object at an index and moves the previous value one up.
   */
  void Insert(const GMath::size_t _index, const T &_value) {
    if (_index == Size()) {
      PushBack(_value);
    }
    else {
      m_values.insert(m_values.begin() + _index, _value);
    }
  }

  /*
   * Erase elements from the array.
   */
  void Erase(const GMath::size_t _index, const GMath::size_t _count = 1) {
    m_values.erase(m_values.begin() + _index, m_values.begin() + _index + _count);
  }

  /*
   * Return the object at an index in the array.
   */
  [[nodiscard]]
  const T &operator[](const GMath::size_t _n) const {
    return m_values[_n];
  };

  /*
   * Return the object at an index in the array.
   */
  [[nodiscard]]
  T &operator[](const GMath::size_t _n) {
    return m_values[_n];
  };
};
} // namespace GMath

/*
 * Stream overload to write GMath::DynamicArray objects.
 */
template <typename value_t>
std::ostream &operator<<(std::ostream &_stream, const GMath::DynamicArray<value_t> &_array) {

  _stream << "[ ";
  for (GMath::size_t __i = 0; __i < _array.Size(); __i++) {
    _stream << _array[__i];

    if (__i != _array.Size() - 1) {
      _stream << ", ";
    }
  }
  _stream << " ]";

  return _stream;
}
