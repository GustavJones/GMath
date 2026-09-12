#pragma once
#include "GMath/Types.hpp"
#include <initializer_list>
#include <iostream>
#include <stdexcept>

namespace GMath {
/*
 * An common interface for a dynamic array.
 */
template <typename T> class DynamicArray {
private:
	T *m_data = nullptr;
	GMath::size_t m_reserve = 0;
	GMath::size_t m_size = 0;

public:
  DynamicArray() = default;

  explicit DynamicArray(const GMath::size_t _n) { Resize(_n); };

  DynamicArray(const T &_value) { PushBack(_value); }

  DynamicArray(const std::initializer_list<T> &_list) {
		for (const auto &item : _list) {
			EmplaceBack(item);
		}
	};

  DynamicArray(DynamicArray &&_arr) : m_data(_arr.m_data), m_size(_arr.m_size), m_reserve(_arr.m_reserve) {
		_arr.m_data = nullptr;
		_arr.m_size = 0;
		_arr.m_reserve = 0;
	}

  DynamicArray(const DynamicArray &_arr) : m_reserve(_arr.m_reserve), m_size(_arr.m_size) {
		m_data = new T[m_reserve];

		for (GMath::size_t i = 0; i < m_size; i++) {
			m_data[i] = _arr.m_data[i];
		}
	}

  DynamicArray &operator=(DynamicArray &&_arr) {
		if (&_arr == this) {
			return *this;
		}

		delete[] m_data;

		m_data = _arr.m_data;
		m_size = _arr.m_size;
		m_reserve = _arr.m_reserve;

		_arr.m_data = nullptr;
		_arr.m_size = 0;
		_arr.m_reserve = 0;

		return *this;
	}

  DynamicArray &operator=(const DynamicArray &_arr) {
		if (&_arr == this) {
			return *this;
		}

		delete[] m_data;

		m_size = _arr.m_size;
		m_reserve = _arr.m_reserve;
		m_data = new T[m_reserve];

		for (GMath::size_t i = 0; i < m_size; i++) {
			m_data[i] = _arr.m_data[i];
		}

		return *this;
	}

  ~DynamicArray() { delete[] m_data; }

  /*
   * Get the amount of items in the array.
   */
  [[nodiscard]]
  GMath::size_t Size() const { return m_size; };

	/*
	 * Get data memory address.
	 */
	T *Data() { return m_data; }

	/*
	 * Get data memory address.
	 */
	const T *Data() const { return m_data; }

  /*
   * Set the amount of items in the array.
   */
  void Resize(const GMath::size_t _size) {
		if (m_reserve < _size) {
			Reserve(2 * _size);
		}

		m_size = _size;
	};

	/*
	 * Reserve an amount of space for items in the array.
	 */
	void Reserve(const GMath::size_t _size) {
		if (_size == 0) {
			delete[] m_data;
			m_data = nullptr;
			m_reserve = 0;
			m_size = 0;
			return;
		}

		T* data = new T[_size]();

		for (GMath::size_t i = 0; i < _size && i < m_size; i++) {
			data[i] = std::move(m_data[i]);
		}

		delete[] m_data;
		m_data = data;
		m_reserve = _size;

		if (m_size > _size) {
			m_size = _size;
		}

	};

  /*
   * Reset the array to empty.
   */
  void Clear() { m_size = 0; };

  /*
   * Copy an object to the back of the array.
   */
  void PushBack(const T &_value) { 
		Insert(m_size, _value);
	}

  /*
   * Move an object to the back of the array.
   */
  void PushBack(T &&_value) { 
		Insert(m_size, std::move(_value));
	}

  /*
   * Create an object at the back of the array.
   */
  template <typename... Args> 
	T &EmplaceBack(Args &&..._args) {
		Resize(m_size + 1);

		m_data[m_size - 1] = std::move(T(_args...));

		return m_data[m_size - 1];
  }

  /*
   * Inserts a copy of an object at an index and moves the previous value one up.
   */
  void Insert(const GMath::size_t _index, const T &_value) {
		if (_index < 0 || _index > m_size) {
			throw std::runtime_error("Index out of bounds.");
		}

		Resize(m_size + 1);

		for (GMath::size_t i = m_size - 1; i > _index; i--) {
			m_data[i] = std::move(m_data[i - 1]);
		}

		m_data[_index] = _value;
  }

  /*
   * Inserts a copy of an object at an index and moves the previous value one up.
   */
  void Insert(const GMath::size_t _index, T &&_value) {
		if (_index < 0 || _index > m_size) {
			throw std::runtime_error("Index out of bounds.");
		}

		Resize(m_size + 1);

		for (GMath::size_t i = m_size - 1; i > _index; i--) {
			m_data[i] = std::move(m_data[i - 1]);
		}

		m_data[_index] = std::move(_value);
  }

  /*
   * Erase elements from the array.
   */
  void Erase(const GMath::size_t _index, const GMath::size_t _count = 1) {
		if (_index < 0 || _index >= m_size) {
			throw std::runtime_error("Index out of bounds.");
		}

		if (_count < 1) {
			return;
		}

		for (GMath::size_t i = _index; i < m_size - _count; i++) {
			m_data[i] = std::move(m_data[i + _count]);
		}

		m_size -= _count;
  }

  /*
   * Return the object at an index in the array.
   */
  [[nodiscard]]
  const T &operator[](const GMath::size_t _n) const {
    return m_data[_n];
  };

  /*
   * Return the object at an index in the array.
   */
  [[nodiscard]]
  T &operator[](const GMath::size_t _n) {
    return m_data[_n];
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
