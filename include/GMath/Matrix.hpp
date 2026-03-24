#pragma once
#include "GMath/DynamicArray.hpp"
#include "GMath/Types.hpp"
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <stdexcept>

namespace GMath {
struct MatrixShape {
  GMath::size_t Rows = 0;
  GMath::size_t Columns = 0;

  MatrixShape() = default;
  MatrixShape(const GMath::size_t _rows, const GMath::size_t _columns)
      : Rows(_rows), Columns(_columns) {};
  MatrixShape(MatrixShape &&) = default;
  MatrixShape(const MatrixShape &) = default;
  MatrixShape &operator=(MatrixShape &&) = default;
  MatrixShape &operator=(const MatrixShape &) = default;
  ~MatrixShape() = default;

  bool operator==(const MatrixShape &_shape) {
    return _shape.Rows == Rows && _shape.Columns == Columns;
  }

  bool operator==(MatrixShape &&_shape) {
    return _shape.Rows == Rows && _shape.Columns == Columns;
  }

  bool operator!=(const MatrixShape &_shape) {
    return _shape.Rows != Rows || _shape.Columns != Columns;
  }

  bool operator!=(MatrixShape &&_shape) {
    return _shape.Rows != Rows || _shape.Columns != Columns;
  }
};

template <typename value_t>
class Matrix : private DynamicArray<DynamicArray<value_t>> {
private:
  bool IsValid() const noexcept {
    GMath::size_t columns = 0;
    GMath::size_t rows = DynamicArray<DynamicArray<value_t>>::Size();

    if (rows > 0) {
      columns = DynamicArray<DynamicArray<value_t>>::operator[](0).Size();
    }

    for (GMath::size_t __row = 0; __row < rows; __row++) {
      const DynamicArray<value_t> &row =
          DynamicArray<DynamicArray<value_t>>::operator[](__row);

      if (columns != row.Size()) {
        return false;
      }
    }

    return true;
  }

public:
  Matrix() {
    Reshape(MatrixShape(0, 0));
  }

  explicit Matrix(const GMath::size_t _rows, const GMath::size_t _columns) {
    Reshape(MatrixShape(_rows, _columns));
  };

  explicit Matrix(const MatrixShape &_shape) {
    Reshape(_shape);
  }

  Matrix(const value_t &_value) : DynamicArray<DynamicArray<value_t>>({_value}) {}
  Matrix(const std::initializer_list<DynamicArray<value_t>> &_list) : DynamicArray<DynamicArray<value_t>>(_list) {}

  Matrix(Matrix &&) = default;
  Matrix(const Matrix &) = default;
  Matrix &operator=(Matrix &&) = default;
  Matrix &operator=(const Matrix &) = default;
  ~Matrix() = default;

  void Identity() {
    MatrixShape shape = Shape();
    if (shape.Rows != shape.Columns) {
      throw std::runtime_error("Matrix rows and columns count do not match.");
    }

    for (GMath::size_t __row = 0; __row < shape.Rows; __row++) {
      for (GMath::size_t __column = 0; __column < shape.Columns; __column++) {
        if (__row == __column) {
          DynamicArray<DynamicArray<value_t>>::operator[](__row)[__column] = 1;
        } else {
          DynamicArray<DynamicArray<value_t>>::operator[](__row)[__column] = 0;
        }
      }
    }
  }

  void Zero() {
    MatrixShape shape = Shape();
    for (GMath::size_t __row = 0; __row < shape.Rows; __row++) {
      for (GMath::size_t __column = 0; __column < shape.Columns; __column++) {
        DynamicArray<DynamicArray<value_t>>::operator[](__row)[__column] = 0;
      }
    }
  }

  virtual void Reshape(const MatrixShape &_shape) {
    DynamicArray<DynamicArray<value_t>>::Resize(_shape.Rows);

    for (GMath::size_t __i = 0;
         __i < DynamicArray<DynamicArray<value_t>>::Size(); __i++) {
      DynamicArray<value_t> &row =
          DynamicArray<DynamicArray<value_t>>::operator[](__i);
      row.Resize(_shape.Columns);
    }

    Zero();
  }

  [[nodiscard]]
  Matrix Round(value_t (*_roundFunc)(value_t _value) = std::round) {
    Matrix temp = *this;
    MatrixShape shape = Shape();
    for (GMath::size_t __row = 0; __row < shape.Rows; __row++) {
      for (GMath::size_t __column = 0; __column < shape.Columns; __column++) {
        value_t &value = temp[__row][__column];
        value = _roundFunc(value);
      }
    }

    return temp;
  }

  [[nodiscard]]
  MatrixShape Shape() const {
    if (!IsValid()) {
      throw std::runtime_error("Invalid matrix.");
    }

    MatrixShape shape;
    shape.Rows = DynamicArray<DynamicArray<value_t>>::Size();

    if (shape.Rows > 0) {
      shape.Columns = DynamicArray<DynamicArray<value_t>>::operator[](0).Size();
    }

    return shape;
  }

  Matrix InsertRow(const DynamicArray<value_t> &_row, const GMath::size_t _index) {
    auto shape = Shape();
    if (_index >= shape.Rows || _index < 0) {
      throw std::runtime_error("Index out of bounds.");
    }

    Matrix<value_t> output = *this;
    output.Insert(_index, _row);
    output[_index].Resize(shape.Columns);

    return output;
  }

  Matrix RemoveRow(const GMath::size_t _index) const {
    auto shape = Shape();
    if (_index >= shape.Rows || _index < 0) {
      throw std::runtime_error("Index out of bounds.");
    }

    Matrix<value_t> output = *this;
    output.Erase(_index);

    return output;
  }

  Matrix AppendRow(const DynamicArray<value_t> &_row) {
    auto shape = Shape();

    Matrix<value_t> output = *this;

    if (shape.Rows > 0) {
      output.InsertRow(_row, shape.Rows - 1);
    }
    else {
      output.InsertRow(_row, 0);
    }

    return output;
  }

  // template<typename... Args>
  // Matrix AppendRow(Args&& ... _args) {
  //   return AppendRow(GMath::DynamicArray<value_t>(_args...));
  // }

  Matrix InsertColumn(const DynamicArray<value_t> &_column, const GMath::size_t _index) {
    auto shape = Shape();
    if (_index >= shape.Columns || _index < 0) {
      throw std::runtime_error("Index out of bounds.");
    }

    Matrix<value_t> output = *this;

    for (size_t __rowIndex = 0; __rowIndex < shape.Rows; __rowIndex++) {
      output[__rowIndex].Insert(_index, _column[__rowIndex]);      
    }

    return output;
  }

  Matrix RemoveColumn(const GMath::size_t _index) const {
    auto shape = Shape();
    if (_index >= shape.Columns || _index < 0) {
      throw std::runtime_error("Index out of bounds.");
    }

    Matrix<value_t> output = *this;

    for (size_t __rowIndex = 0; __rowIndex < shape.Rows; __rowIndex++) {
      output[__rowIndex].Erase(_index);
    }

    return output;
  }

  Matrix AppendColumn(const DynamicArray<value_t> &_column) {
    auto shape = Shape();

    Matrix<value_t> output = *this;

    if (shape.Columns > 0) {
      output.InsertColumn(_column, shape.Columns - 1);
    }
    else {
      output.InsertColumn(_column, 0);
    }

    return output;
  }

  // template<typename... Args>
  // Matrix AppendColumn(Args&& ... _args) {
  //   return AppendColumn(GMath::DynamicArray<value_t>(_args...));
  // }

  [[nodiscard]]
  Matrix Slice(const GMath::size_t _rowIndex, const GMath::size_t _columnIndex, const GMath::size_t _rowCount, const GMath::size_t _columnCount) const {
    auto shape = Shape();
    if (_rowIndex + _rowCount > shape.Rows) {
      throw std::runtime_error("Slice row count outside matrix size.");
    }
    else if (_columnIndex + _columnCount > shape.Columns) {
      throw std::runtime_error("Slice column count outside matrix size.");
    }
    else if (_rowIndex < 0) {
      throw std::runtime_error("Slice row index outside matrix size.");
    }
    else if (_columnIndex < 0) {
      throw std::runtime_error("Slice column index outside matrix size.");
    }

    Matrix<value_t> output(_rowCount, _columnCount);

    for (size_t __rIndex = 0; __rIndex < _rowCount; __rIndex++) {
      for (size_t __cIndex = 0; __cIndex < _columnCount; __cIndex++) {
        output[__rIndex][__cIndex] = operator[](_rowIndex + __rIndex)[_columnIndex + __cIndex];
      }
    }

    return output;
  }

  Matrix Transpose() const {
    auto shape = Shape();
    Matrix<value_t> output(shape.Columns, shape.Rows);

    for (size_t __rowIndex = 0; __rowIndex < shape.Rows; __rowIndex++) {
      for (size_t __columnIndex = 0; __columnIndex < shape.Columns; __columnIndex++) {
        output[__columnIndex][__rowIndex] = operator[](__rowIndex)[__columnIndex];
      }
    }

    return output;
  }

  Matrix Inverse() const {
    MatrixShape shape = Shape();
    Matrix<value_t> temp = *this;
    Matrix<value_t> output(shape);
    output.Identity();

    if (shape.Rows != shape.Columns) {
      throw std::runtime_error("Cannot compute inverse matrix.");
    }

    value_t factor;

    for (GMath::size_t __row = 0; __row < shape.Rows; __row++) {
      // Get diagonal factor
      factor = temp[__row][__row];

      // R <- (1 / factor) * R
      // To set the diagonal value to 1
      // Will be used to multipy with the factor of each row in the column to elimate the value
      for (GMath::size_t __column = 0; __column < shape.Columns; __column++) {
        temp[__row][__column] = temp[__row][__column] / factor;
        output[__row][__column] = output[__row][__column] / factor;
      }

      // Eliminate previous rows to 0
      for (GMath::size_t __prevRow = 0; __prevRow < __row; __prevRow++) {
        // Works with column linked to diagonal row index 
        factor = temp[__prevRow][__row];

        for (GMath::size_t __changeColumn = 0; __changeColumn < shape.Columns; __changeColumn++) {
          temp[__prevRow][__changeColumn] -= temp[__row][__changeColumn] * factor;
          output[__prevRow][__changeColumn] -= output[__row][__changeColumn] * factor;
        }
      }

      // Eliminate next rows to 0
      for (GMath::size_t __nextRow = __row + 1; __nextRow < shape.Rows; __nextRow++) {
        // Works with column linked to diagonal row index 
        factor = temp[__nextRow][__row];

        for (GMath::size_t __changeColumn = 0; __changeColumn < shape.Columns; __changeColumn++) {
          temp[__nextRow][__changeColumn] -= temp[__row][__changeColumn] * factor;
          output[__nextRow][__changeColumn] -= output[__row][__changeColumn] * factor;
        }
      }
    }

    return output;
  }

  value_t Determinant() const {
    value_t output = 0;
    auto shape = Shape();

    if (shape.Rows != shape.Columns) {
      throw std::runtime_error("Cannot calculate the determinant of a non-square matrix.");
    }

    if (shape.Rows == 2) {
      output = operator[](0)[0] * operator[](1)[1] - operator[](0)[1] * operator[](1)[0];
    }
    else {
      Matrix<value_t> temp = *this;
      for (size_t __columnIndex = 0; __columnIndex < shape.Columns; __columnIndex++) {
        auto factor = temp[0][__columnIndex];
        factor *= temp.RemoveRow(0).RemoveColumn(__columnIndex).Determinant();

        if (__columnIndex % 2 == 0) {
          output += factor;
        }
        else {
          output -= factor;
        }
      }
    }

    return output;
  }

  Matrix operator+(const Matrix<value_t> &_matrix) const {
    Matrix<value_t> output = *this;

    auto thisShape = Shape();
    auto otherShape = _matrix.Shape();

    if (thisShape != otherShape) {
      throw std::runtime_error("Matrix shapes not the same.");
    }

    for (GMath::size_t __row = 0; __row < thisShape.Rows; __row++) {
      for (GMath::size_t __column = 0; __column < thisShape.Columns;
           __column++) {
        output[__row][__column] += _matrix[__row][__column];
      }
    }

    return output;
  }

  Matrix operator-(const Matrix<value_t> &_matrix) const {
    Matrix<value_t> output = *this;

    auto thisShape = Shape();
    auto otherShape = _matrix.Shape();

    if (thisShape != otherShape) {
      throw std::runtime_error("Matrix shapes not the same.");
    }

    for (GMath::size_t __row = 0; __row < thisShape.Rows; __row++) {
      for (GMath::size_t __column = 0; __column < thisShape.Columns;
           __column++) {
        output[__row][__column] -= _matrix[__row][__column];
      }
    }

    return output;
  }

  Matrix operator*(const value_t _value) const {
    auto shape = Shape();
    Matrix<value_t> output(shape.Rows, shape.Columns);

    for (GMath::size_t __row = 0; __row < shape.Rows; __row++) {
      for (GMath::size_t __column = 0; __column < shape.Columns; __column++) {
        output[__row][__column] = DynamicArray<DynamicArray<value_t>>::operator[](__row)[__column] * _value;
      }
    }

    return output;
  }

  Matrix operator*(const Matrix<value_t> &_matrix) const {
    auto thisShape = Shape();
    auto otherShape = _matrix.Shape();

    Matrix<value_t> output(thisShape.Rows, otherShape.Columns);

    if (thisShape.Columns != otherShape.Rows) {
      throw std::runtime_error("Matrix shapes cannot be multiplied.");
    }

    for (GMath::size_t __row = 0; __row < thisShape.Rows; __row++) {
      for (GMath::size_t __column = 0; __column < otherShape.Columns;
           __column++) {
        for (GMath::size_t __index = 0; __index < thisShape.Columns;
             __index++) {
          output[__row][__column] +=
              DynamicArray<DynamicArray<value_t>>::operator[](__row)[__index] *
              _matrix[__index][__column];
        }
      }
    }

    return output;
  }

  Matrix operator/(const value_t _value) const {
    return *this * (1 / _value);
  }

  Matrix operator/(const Matrix<value_t> &_matrix) const {
    return *this * _matrix.Inverse();
  }

  DynamicArray<value_t> &operator[](const GMath::size_t _n) {
    return DynamicArray<DynamicArray<value_t>>::operator[](_n);
  }

  const DynamicArray<value_t> &operator[](const GMath::size_t _n) const {
    return DynamicArray<DynamicArray<value_t>>::operator[](_n);
  }
};
} // namespace GMath

// Printing matrices
template <typename value_t>
std::ostream &operator<<(std::ostream &_stream, const GMath::Matrix<value_t> &_matrix) {
  GMath::MatrixShape shape = _matrix.Shape();

  for (GMath::size_t __i = 0; __i < shape.Rows; __i++) {
    _stream << "| ";
    for (GMath::size_t __j = 0; __j < shape.Columns; __j++) {
      _stream << _matrix[__i][__j] << ' ';
    }
    _stream << '|' << std::endl;
  }

  return _stream;
}

// Here to allow n * Matrix as well (Matrix * n is the only version that can be implemented in the class)
template<typename scalar_t, typename value_t>
GMath::Matrix<value_t> operator*(const scalar_t _value, GMath::Matrix<value_t> &_matrix) {
  return _matrix * _value;
}
