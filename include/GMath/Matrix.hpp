#pragma once
#include "GMath/DynamicArray.hpp"
#include "GMath/Types.hpp"
#include <cmath>
#include <functional>
#include <future>
#include <iostream>
#include <stdexcept>

namespace GMath {
/*
 * A container for the shape of a Matrix.
 * It contains the Rows and Columns of a Matrix.
 */
struct MatrixShape {
  GMath::size_t Rows = 0;
  GMath::size_t Columns = 0;

  MatrixShape() = default;
  MatrixShape(const GMath::size_t _rows, const GMath::size_t _columns) : Rows(_rows), Columns(_columns) {};
  MatrixShape(MatrixShape &&) = default;
  MatrixShape(const MatrixShape &) = default;
  MatrixShape &operator=(MatrixShape &&) = default;
  MatrixShape &operator=(const MatrixShape &) = default;
  ~MatrixShape() = default;

  /*
   * Compare operator overload.
   */
  bool operator==(const MatrixShape &_shape) {
    return _shape.Rows == Rows && _shape.Columns == Columns;
  }

  /*
   * Compare operator overload.
   */
  bool operator==(MatrixShape &&_shape) {
    return _shape.Rows == Rows && _shape.Columns == Columns;
  }

  /*
   * Compare operator overload.
   */
  bool operator!=(const MatrixShape &_shape) {
    return _shape.Rows != Rows || _shape.Columns != Columns;
  }

  /*
   * Compare operator overload.
   */
  bool operator!=(MatrixShape &&_shape) {
    return _shape.Rows != Rows || _shape.Columns != Columns;
  }
};

/*
 * A class for a Matrix structure in C++.
 */
template <typename value_t>
class Matrix : private DynamicArray<DynamicArray<value_t>> {
private:
  /*
   * Check if the Matrix has a constant amount of columns in each row.
   * If false, the Matrix has a row that doesn't have the same amount of columns as the first row.
   */
  bool IsValid() const noexcept {
    GMath::size_t columns = 0;
    GMath::size_t rows = DynamicArray<DynamicArray<value_t>>::Size();

    if (rows > 0) {
      columns = DynamicArray<DynamicArray<value_t>>::operator[](0).Size();

      for (GMath::size_t __row = 0; __row < rows; __row++) {
        const DynamicArray<value_t> &row = DynamicArray<DynamicArray<value_t>>::operator[](__row);

        if (columns != row.Size()) {
          return false;
        }
      }
    }

    return true;
  }

public:
  /*
   * Create an empty Matrix.
   */
  Matrix() { Reshape(MatrixShape(0, 0)); }

  /*
   * Create a row matrix from an array.
   */
  Matrix(const GMath::DynamicArray<value_t> &_arr) : DynamicArray<DynamicArray<value_t>>({_arr}) {}

  /*
   * Create a matrix with a specific size.
   */
  explicit Matrix(const GMath::size_t _rows, const GMath::size_t _columns) {
    Reshape(MatrixShape(_rows, _columns));
  };

  /*
   * Create a matrix with a specific size.
   */
  explicit Matrix(const MatrixShape &_shape) { Reshape(_shape); }

  /*
   * Create a 1x1 matrix with the value.
   */
  Matrix(const value_t &_value) : DynamicArray<DynamicArray<value_t>>({_value}) {}

  /*
   * Create a matrix from an initializer list.
   */
  Matrix(const std::initializer_list<DynamicArray<value_t>> &_list) : DynamicArray<DynamicArray<value_t>>(_list) {}

  Matrix(Matrix &&) = default;
  Matrix(const Matrix &) = default;
  Matrix &operator=(Matrix &&) = default;
  Matrix &operator=(const Matrix &) = default;

  ~Matrix() = default;

  /*
   * Checks if the matrix has only 1 column.
   */
  bool IsColumnMatrix() const {
    return Shape().Columns == 1;
  }

  /*
   * Checks if the matrix has only 1 row.
   */
  bool IsRowMatrix() const {
    return Shape().Rows == 1;
  }

  /*
   * Creates an identity matrix with the current size.
   * If the size is not square, it will throw an exception.
   */
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

  /*
   * Set all of the values of the current matrix to 0.
   */
  void Zero() {
    MatrixShape shape = Shape();
    for (GMath::size_t __row = 0; __row < shape.Rows; __row++) {
      for (GMath::size_t __column = 0; __column < shape.Columns; __column++) {
        DynamicArray<DynamicArray<value_t>>::operator[](__row)[__column] = 0;
      }
    }
  }

  /*
   * Resizes the matrix and Zero().
   */
  virtual void Reshape(const MatrixShape &_shape) {
    DynamicArray<DynamicArray<value_t>>::Resize(_shape.Rows);

    for (GMath::size_t __i = 0; __i < DynamicArray<DynamicArray<value_t>>::Size(); __i++) {
      DynamicArray<value_t> &row = DynamicArray<DynamicArray<value_t>>::operator[](__i);
      row.Resize(_shape.Columns);
    }

    Zero();
  }

  /*
   * Round each value of the matrix with a specific rounding function.
   */
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

  /*
   * Get the MatrixShape of the matrix.
   */
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

  /*
   * Insert a row into the matrix at an index, moving up all of the other rows (including the old row at the index).
   */
  [[nodiscard]]
  Matrix InsertRow(const DynamicArray<value_t> &_row, const GMath::size_t _index) {
    auto shape = Shape();
    if (_index > shape.Rows || _index < 0) {
      throw std::runtime_error("Index out of bounds.");
    }

    Matrix<value_t> output = *this;
    output.Insert(_index, _row);

    if (!IsValid()) {
      throw std::runtime_error("Invalid row inserted...");
    }

    return output;
  }

  /*
   * Remove a row from the matrix at an index, moving up all of the other rows.
   */
  [[nodiscard]]
  Matrix RemoveRow(const GMath::size_t _index) const {
    auto shape = Shape();
    if (_index >= shape.Rows || _index < 0) {
      throw std::runtime_error("Index out of bounds.");
    }

    Matrix<value_t> output = *this;
    output.Erase(_index);

    return output;
  }
  /*
   * Insert a row into the matrix at the end.
   */
  [[nodiscard]]
  Matrix AppendRow(const DynamicArray<value_t> &_row) {
    auto shape = Shape();

    Matrix<value_t> output = *this;

    output = output.InsertRow(_row, shape.Rows);
    return output;
  }

  /*
   * Insert a column into the matrix at an index, moving up all of the other columns (including the old column at the index).
   */
  [[nodiscard]]
  Matrix InsertColumn(const DynamicArray<value_t> &_column, const GMath::size_t _index) {
    auto shape = Shape();

    if (_index > shape.Columns || _index < 0) {
      throw std::runtime_error("Index out of bounds.");
    }

    Matrix<value_t> output = *this;

    if (shape.Rows == 0) {
      output.Reshape({_column.Size(), 0});
      shape.Rows = _column.Size();
    }

    if (_column.Size() != shape.Rows) {
      throw std::runtime_error("Inserted invalid column...");
    }

    for (size_t __rowIndex = 0; __rowIndex < shape.Rows; __rowIndex++) {
      output[__rowIndex].Insert(_index, _column[__rowIndex]);
    }

    return output;
  }

  /*
   * Remove a column from the matrix at an index, moving up all of the other columns.
   */
  [[nodiscard]]
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

  /*
   * Insert a column into the matrix at the end.
   */
  [[nodiscard]]
  Matrix AppendColumn(const DynamicArray<value_t> &_column) {
    auto shape = Shape();

    Matrix<value_t> output = *this;

    output = output.InsertColumn(_column, shape.Columns);
    return output;
  }

  /*
   * Create a sub-matrix within the original matrix.
   */
  [[nodiscard]]
  Matrix Slice(const GMath::size_t _rowIndex, 
               const GMath::size_t _columnIndex,
               const GMath::size_t _rowCount,
               const GMath::size_t _columnCount) const {
    auto shape = Shape();
    if (_rowIndex + _rowCount > shape.Rows) {
      throw std::runtime_error("Slice row count outside matrix size.");
    } else if (_columnIndex + _columnCount > shape.Columns) {
      throw std::runtime_error("Slice column count outside matrix size.");
    } else if (_rowIndex < 0) {
      throw std::runtime_error("Slice row index outside matrix size.");
    } else if (_columnIndex < 0) {
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

  /*
   * Swap rows and columns.
   */
  [[nodiscard]]
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

  /*
   * Get the inverse matrix.
   * If T is the matrix, T^-1 would be the inverse so:
   * T^-1*T = 1
   */
  [[nodiscard]]
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
      // Will be used to multipy with the factor of each row in the column to
      // elimate the value
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

  /*
   * Get the determinant of the matrix.
   */
  [[nodiscard]]
  value_t Determinant() const {
    value_t output = 0;
    auto shape = Shape();

    if (shape.Rows != shape.Columns) {
      throw std::runtime_error("Cannot calculate the determinant of a non-square matrix.");
    }

    if (shape.Rows == 2) {
      output = operator[](0)[0] * operator[](1)[1] - operator[](0)[1] * operator[](1)[0];
    } else {
      Matrix<value_t> temp = *this;
      for (size_t __columnIndex = 0; __columnIndex < shape.Columns; __columnIndex++) {
        auto factor = temp[0][__columnIndex];
        factor *= temp.RemoveRow(0).RemoveColumn(__columnIndex).Determinant();

        if (__columnIndex % 2 == 0) {
          output += factor;
        } else {
          output -= factor;
        }
      }
    }

    return output;
  }

  /*
   * Add a matrices of the same shape. 
   */
  [[nodiscard]]
  Matrix operator+(const Matrix<value_t> &_matrix) const {
    Matrix<value_t> output = *this;

    auto thisShape = Shape();
    auto otherShape = _matrix.Shape();

    if (thisShape != otherShape) {
      throw std::runtime_error("Matrix shapes not the same.");
    }

    for (GMath::size_t __row = 0; __row < thisShape.Rows; __row++) {
      for (GMath::size_t __column = 0; __column < thisShape.Columns; __column++) {
        output[__row][__column] += _matrix[__row][__column];
      }
    }

    return output;
  }

  /*
   * Subtract matrices of the same shape.
   */
  [[nodiscard]]
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

  /*
   * Multiply the matrix with a scalar.
   */
  [[nodiscard]]
  Matrix operator*(const value_t _value) const {
    static const bool THREADING_ENABLED = false;
    static const GMath::size_t THREADING_LIMIT = 16;
    static const GMath::size_t THREADING_COUNT = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 1;

    auto shape = Shape();
    Matrix<value_t> output(shape.Rows, shape.Columns);

    DynamicArray<std::future<void>> threads {THREADING_COUNT};
    const auto batchFunc = [](const Matrix<value_t> &_source, Matrix<value_t> &_output, const MatrixShape &_shape, const GMath::size_t _rowStart, const GMath::size_t _rowEnd, const value_t _value) {
      for (size_t __row = _rowStart; __row < _rowEnd; __row++) {
        for (GMath::size_t __column = 0; __column < _shape.Columns; __column++) {
          _output[__row][__column] = _source[__row][__column] * _value;
        }
      }
    };

    if (shape.Rows > THREADING_LIMIT && THREADING_ENABLED) {
      const GMath::size_t batchSize { shape.Rows / threads.Size() + 1};
      for (GMath::size_t __threadIndex = 0; __threadIndex < threads.Size(); __threadIndex++) {
        const auto batchStart = batchSize * __threadIndex, batchEnd = (batchStart + batchSize) > shape.Rows ? shape.Rows : (batchStart + batchSize);
        threads[__threadIndex] = std::async(std::launch::async, batchFunc, std::ref(*this), std::ref(output), std::ref(shape), batchStart, batchEnd, _value);
      }

      for (GMath::size_t __threadIndex = 0; __threadIndex < threads.Size(); __threadIndex++) {
        threads[__threadIndex].get();
      }
    }
    else {
      batchFunc(*this, output, shape, 0, shape.Rows, _value);
    }


    return output;
  }

  /*
   * Multiply matrices of suitable shapes.
   */
  [[nodiscard]]
  Matrix operator*(const Matrix<value_t> &_matrix) const {
    static const bool THREADING_ENABLED = false;
    static const GMath::size_t THREADING_LIMIT = 16;
    static const GMath::size_t THREADING_COUNT = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 1;
    auto thisShape = Shape();
    auto otherShape = _matrix.Shape();

    Matrix<value_t> output(thisShape.Rows, otherShape.Columns);

    if (thisShape.Columns != otherShape.Rows) {
      throw std::runtime_error("Matrix shapes cannot be multiplied.");
    }

    DynamicArray<std::future<void>> threads {THREADING_COUNT};
    const auto batchFunc = [](const Matrix<value_t> &_matrix1, const Matrix<value_t> &_matrix2, Matrix<value_t> &_output, const MatrixShape &_matrix1Shape, const MatrixShape &_matrix2Shape, const GMath::size_t _rowStart, const GMath::size_t _rowEnd) {
      for (GMath::size_t __row = _rowStart; __row < _rowEnd; __row++) {
        for (GMath::size_t __column = 0; __column < _matrix2Shape.Columns; __column++) {
          for (GMath::size_t __index = 0; __index < _matrix1Shape.Columns; __index++) {
            _output[__row][__column] += _matrix1[__row][__index] * _matrix2[__index][__column];
          }
        }
      }
    };

    if (thisShape.Rows > THREADING_LIMIT && THREADING_ENABLED) {
      const GMath::size_t batchSize = { thisShape.Rows / threads.Size() + 1};
      for (GMath::size_t __threadIndex = 0; __threadIndex < threads.Size(); __threadIndex++) {
        const auto batchStart = batchSize * __threadIndex, batchEnd = (batchStart + batchSize) > thisShape.Rows ? thisShape.Rows : (batchStart + batchSize);
        threads[__threadIndex] = std::async(std::launch::async, batchFunc, std::ref(*this), std::ref(_matrix), std::ref(output), std::ref(thisShape), std::ref(otherShape), batchStart, batchEnd);
      }

      for (GMath::size_t __threadIndex = 0; __threadIndex < threads.Size(); __threadIndex++) {
        threads[__threadIndex].get();
      }
    }
    else {
      batchFunc(*this, _matrix, output, thisShape, otherShape, 0, thisShape.Rows);
    }


    return output;
  }

  /*
   * Multiply the matrix with 1/scalar.
   */
  [[nodiscard]]
  Matrix operator/(const value_t _value) const { return *this * (1 / _value); }

  /*
   * Multiply a matrix by another's inverse.
   */
  [[nodiscard]]
  Matrix operator/(const Matrix<value_t> &_matrix) const {
    return *this * _matrix.Inverse();
  }

  /*
   * Get an array of a row in the matrix.
   */
  [[nodiscard]]
  DynamicArray<value_t> &operator[](const GMath::size_t _n) {
    return DynamicArray<DynamicArray<value_t>>::operator[](_n);
  }

  /*
   * Get an array of a row in the matrix.
   */
  [[nodiscard]]
  const DynamicArray<value_t> &operator[](const GMath::size_t _n) const {
    return DynamicArray<DynamicArray<value_t>>::operator[](_n);
  }
};
} // namespace GMath

/*
 * Stream override to write a matrix.
 */
template <typename value_t>
std::ostream &operator<<(std::ostream &_stream, const GMath::Matrix<value_t> &_matrix) {
  GMath::MatrixShape shape = _matrix.Shape();

  if (shape.Rows == 0) {
    _stream << "[]";
    return _stream;
  }

  _stream << "[" << std::endl;
  for (GMath::size_t __i = 0; __i < shape.Rows; __i++) {
    _stream << '\t' << _matrix[__i] << std::endl;
  }
  _stream << ']';

  return _stream;
}

/*
 * Multiply a scalar with a matrix.
 * (Should be the same as matrix * scalar)
 */
template <typename value_t>
[[nodiscard]]
GMath::Matrix<value_t> operator*(const value_t _value, GMath::Matrix<value_t> &_matrix) {
  return _matrix * _value;
}
