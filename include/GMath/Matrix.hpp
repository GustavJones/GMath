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

template <typename value_t>
class Matrix;

/*
 * A class for a Matrix row.
 */
template <typename value_t>
class MatrixRow {
private:
	DynamicArray<value_t> m_columns;

protected:
	friend Matrix<value_t>;

	/*
	 * Resize the row.
	 */
	void Resize(const GMath::size_t _size) { m_columns.Resize(_size); }

	/*
	 * Insert a column into the row.
	 */
	void Insert(const GMath::size_t _index, const value_t _value) { m_columns.Insert(_index, _value); }


	/*
	 * Erase a column form the row.
	 */
	void Erase(const GMath::size_t _index, const GMath::size_t _count = 1) { m_columns.Erase(_index, _count); }

public:
	MatrixRow() = default;

	MatrixRow(const DynamicArray<value_t> &_arr) : m_columns(_arr) {}
	MatrixRow(DynamicArray<value_t> &&_arr) : m_columns(_arr) {}

  explicit MatrixRow(const GMath::size_t _n) : m_columns(_n) {};
  MatrixRow(const value_t &_value) { m_columns.PushBack(_value); }
  MatrixRow(const std::initializer_list<value_t> &_list) : m_columns(_list) {};

	MatrixRow(const MatrixRow &_row) = default;
	MatrixRow(MatrixRow &&_row) = default;
	MatrixRow &operator=(const MatrixRow &_row) = default;
	MatrixRow &operator=(MatrixRow &&_row) = default;
	~MatrixRow() = default;

	/*
	 * Get the size of the row.
	 */
	[[nodiscard]]
	GMath::size_t Size() const { return m_columns.Size(); }

	/*
	 * Get the value of a column in the row.
	 */
	[[nodiscard]]
	value_t &operator[](const GMath::size_t _column) { return m_columns[_column]; }

	/*
	 * Get the value of a column in the row.
	 */
	[[nodiscard]]
	const value_t &operator[](const GMath::size_t _column) const { return m_columns[_column]; }

	explicit operator const DynamicArray<value_t> &() const { return m_columns; }
};

/*
 * A class for a Matrix structure in C++.
 */
template <typename value_t>
class Matrix {
private:
	DynamicArray<MatrixRow<value_t>> m_rows;

  /*
   * Check if the Matrix has a constant amount of columns in each row.
   * If false, the Matrix has a row that doesn't have the same amount of columns as the first row.
   */
	[[deprecated("No longer needed.")]]
	[[nodiscard]]
  bool _IsValid() const noexcept {
    GMath::size_t columns = 0;
    GMath::size_t rows = m_rows.Size();

    if (rows > 0) {
      columns = m_rows[0].Size();

      for (GMath::size_t __row = 1; __row < rows; __row++) {
        const auto &row = m_rows[__row];

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
   * Create a row matrix from a row.
   */
  Matrix(const MatrixRow<value_t> &_row) { AppendRow((DynamicArray<value_t>)_row); }

  /*
   * Create a row matrix from an array.
   */
  Matrix(const DynamicArray<value_t> &_row) { AppendRow(_row); }

  /*
   * Create a matrix with a specific size.
   */
  explicit Matrix(const GMath::size_t _rows, const GMath::size_t _columns) { Reshape(MatrixShape(_rows, _columns)); };

  /*
   * Create a matrix with a specific size.
   */
  explicit Matrix(const MatrixShape &_shape) { Reshape(_shape); }

  /*
   * Create a 1x1 matrix with the value.
   */
  Matrix(const value_t &_value) {
		Reshape({1, 1});
		m_rows[0][0] = _value;
	}

  /*
   * Create a matrix from an initializer list.
   */
  Matrix(const std::initializer_list<MatrixRow<value_t>> &_list) : m_rows(_list) {}

  /*
   * Checks if the matrix has only 1 column.
   */
	[[nodiscard]]
  bool IsColumnMatrix() const {
    return Shape().Columns == 1;
  }

  /*
   * Checks if the matrix has only 1 row.
   */
	[[nodiscard]]
  bool IsRowMatrix() const {
    return Shape().Rows == 1;
  }

	/*
	 * Check if the matrix is square.
	 */
	[[nodiscard]]
	bool IsSquare() const {
		auto shape = Shape();
		return shape.Rows == shape.Columns;
	}

	/*
	 * Check if a Matrix has rows and columns.
	 */
	[[nodiscard]]
	bool IsEmpty() const noexcept {
    GMath::size_t columns = 0;
    GMath::size_t rows = m_rows.Size();

		if (rows < 1) return true; 

		for (GMath::size_t __row = 0; __row < rows; __row++) {
			if (m_rows[__row].Size() > 0) {
				return false;
			}
		}

		return true;
	}

  /*
   * Creates an identity matrix with the current size.
   * If the size is not square, it will throw an exception.
   */
  void Identity() {
    MatrixShape shape = Shape();

    if (!IsSquare()) {
      throw std::runtime_error("Not a square matrix.");
    }

    for (GMath::size_t __row = 0; __row < shape.Rows; __row++) {
      for (GMath::size_t __column = 0; __column < shape.Columns; __column++) {
        if (__row == __column) {
					m_rows[__row][__column] = 1;
        } else {
					m_rows[__row][__column] = 0;
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
				m_rows[__row][__column] = 0;
      }
    }
  }

  /*
   * Resizes the matrix.
   */
  virtual void Reshape(const MatrixShape &_shape) {
		m_rows.Resize(_shape.Rows);

    for (GMath::size_t __row = 0; __row < m_rows.Size(); __row++) {
      auto &row = m_rows[__row];
      row.Resize(_shape.Columns);
    }
  }

  /*
   * Round each value of the matrix with a specific rounding function.
   */
  void Round(value_t (*_roundFunc)(value_t _value) = std::round) {
    MatrixShape shape = Shape();

    for (GMath::size_t __row = 0; __row < shape.Rows; __row++) {
      for (GMath::size_t __column = 0; __column < shape.Columns; __column++) {
        m_rows[__row][__column] = _roundFunc(m_rows[__row][__column]);
      }
    }
  }

  /*
   * Get the MatrixShape of the matrix.
   */
  [[nodiscard]]
  MatrixShape Shape() const {
    MatrixShape shape;
    shape.Rows = m_rows.Size();

    if (shape.Rows > 0) {
      shape.Columns = m_rows[0].Size();
    }

    return shape;
  }

  /*
   * Insert a row into the matrix at an index, moving up all of the other rows (including the old row at the index).
   */
  void InsertRow(const DynamicArray<value_t> &_row, const GMath::size_t _index) {
    auto shape = Shape();

    if (_index > shape.Rows || _index < 0) {
      throw std::runtime_error("Index out of bounds.");
    }

		if (_row.Size() != shape.Columns && !IsEmpty()) {
			throw std::runtime_error("Row not the correct size.");
		}

    m_rows.Insert(_index, _row);
  }

  /*
   * Remove a row from the matrix at an index, moving up all of the other rows.
   */
  void RemoveRow(const GMath::size_t _index) {
    auto shape = Shape();

    if (_index >= shape.Rows || _index < 0) {
      throw std::runtime_error("Index out of bounds.");
    }

		m_rows.Erase(_index);
  }

  /*
   * Insert a row into the matrix at the end.
   */
  void AppendRow(const DynamicArray<value_t> &_row) {
    auto shape = Shape();

		InsertRow(_row, shape.Rows);
  }

  /*
   * Insert a column into the matrix at an index, moving up all of the other columns (including the old column at the index).
   */
  void InsertColumn(const DynamicArray<value_t> &_column, const GMath::size_t _index) {
    auto shape = Shape();

    if (_index > shape.Columns || _index < 0) {
      throw std::runtime_error("Index out of bounds.");
    }

		if (_column.Size() != shape.Rows && !IsEmpty()) {
			throw std::runtime_error("Column not the correct size.");
		}

    if (shape.Rows == 0) {
      Reshape({_column.Size(), 0});
			shape = Shape();
    }

    for (size_t __rowIndex = 0; __rowIndex < shape.Rows; __rowIndex++) {
      m_rows[__rowIndex].Insert(_index, _column[__rowIndex]);
    }
  }

  /*
   * Remove a column from the matrix at an index, moving up all of the other columns.
   */
  void RemoveColumn(const GMath::size_t _index) {
    auto shape = Shape();

    if (_index >= shape.Columns || _index < 0) {
      throw std::runtime_error("Index out of bounds.");
    }

    for (size_t __rowIndex = 0; __rowIndex < shape.Rows; __rowIndex++) { m_rows[__rowIndex].Erase(_index); }
  }

  /*
   * Insert a column into the matrix at the end.
   */
	[[deprecated("Untested.")]]
  void AppendColumn(const DynamicArray<value_t> &_column) {
    auto shape = Shape();

		InsertColumn(_column, shape.Columns);
  }

  /*
   * Create a sub-matrix within the original matrix.
   */
  void Slice(const GMath::size_t _rowIndex, const GMath::size_t _columnIndex, const MatrixShape &_newShape) {
    auto shape = Shape();

    if (_rowIndex + _newShape.Rows > shape.Rows) {
      throw std::runtime_error("Slice row count outside matrix size.");
    } else if (_columnIndex + _newShape.Columns > shape.Columns) {
      throw std::runtime_error("Slice column count outside matrix size.");
    } else if (_rowIndex < 0) {
      throw std::runtime_error("Slice row index outside matrix size.");
    } else if (_columnIndex < 0) {
      throw std::runtime_error("Slice column index outside matrix size.");
    }

		auto copy = *this;
		copy.Reshape(_newShape);

    for (size_t __row = 0; __row < _newShape.Rows; __row++) {
      for (size_t __column = 0; __column < _newShape.Columns; __column++) {
        copy[__row][__column] = m_rows[_rowIndex + __row][_columnIndex + __column];
      }
    }

		m_rows = std::move(copy.m_rows);
  }

  /*
   * Swap rows and columns.
   */
  void Transpose() {
    auto shape = Shape();

    Matrix<value_t> output(shape.Columns, shape.Rows);

    for (size_t __rowIndex = 0; __rowIndex < shape.Rows; __rowIndex++) {
      for (size_t __columnIndex = 0; __columnIndex < shape.Columns; __columnIndex++) {
        output[__columnIndex][__rowIndex] = m_rows[__rowIndex][__columnIndex];
      }
    }

		m_rows = std::move(output.m_rows);
  }

  /*
   * Get the inverse matrix.
   * If T is the matrix, T^-1 would be the inverse so:
   * T^-1*T = 1
   */
  void Inverse() {
    MatrixShape shape = Shape();

		if (shape.Rows != shape.Columns) {
			throw std::runtime_error("Not a square matrix.");
		}

    Matrix<value_t> copy = *this;
    Identity();

    value_t factor;

    for (GMath::size_t __row = 0; __row < shape.Rows; __row++) {
      // Get diagonal factor
      factor = copy[__row][__row];

      // R <- (1 / factor) * R
      // To set the diagonal value to 1
      // Will be used to multipy with the factor of each row in the column to
      // elimate the value
      for (GMath::size_t __column = 0; __column < shape.Columns; __column++) {
        copy[__row][__column] = copy[__row][__column] / factor;
        m_rows[__row][__column] = m_rows[__row][__column] / factor;
      }

      // Eliminate previous rows to 0
      for (GMath::size_t __prevRow = 0; __prevRow < __row; __prevRow++) {
        // Works with column linked to diagonal row index
        factor = copy[__prevRow][__row];

        for (GMath::size_t __changeColumn = 0; __changeColumn < shape.Columns; __changeColumn++) {
          copy[__prevRow][__changeColumn] -= copy[__row][__changeColumn] * factor;
          m_rows[__prevRow][__changeColumn] -= m_rows[__row][__changeColumn] * factor;
        }
      }

      // Eliminate next rows to 0
      for (GMath::size_t __nextRow = __row + 1; __nextRow < shape.Rows; __nextRow++) {
        // Works with column linked to diagonal row index
        factor = copy[__nextRow][__row];

        for (GMath::size_t __changeColumn = 0; __changeColumn < shape.Columns; __changeColumn++) {
          copy[__nextRow][__changeColumn] -= copy[__row][__changeColumn] * factor;
          m_rows[__nextRow][__changeColumn] -= m_rows[__row][__changeColumn] * factor;
        }
      }
    }
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
      output = m_rows[0][0] * m_rows[1][1] - m_rows[0][1] * m_rows[1][0];
    } else {
      for (size_t __rowIndex = 0; __rowIndex < shape.Rows; __rowIndex++) {
				Matrix<value_t> temp = *this;
				auto factor = m_rows[__rowIndex][0];
				if (__rowIndex % 2 == 1) factor *= -1;

				temp.RemoveColumn(0);
				temp.RemoveRow(__rowIndex);

				output += factor * temp.Determinant();
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
      for (GMath::size_t __column = 0; __column < thisShape.Columns; __column++) {
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
    static const GMath::size_t THREADING_COUNT = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 1;
    static const GMath::size_t THREADING_LIMIT = THREADING_COUNT * 2;
    auto thisShape = Shape();
    auto otherShape = _matrix.Shape();

    Matrix<value_t> output(thisShape.Rows, otherShape.Columns);

    if (thisShape.Columns != otherShape.Rows) {
      throw std::runtime_error("Matrix shapes cannot be multiplied.");
    }

    DynamicArray<std::future<void>> threads {THREADING_COUNT};
    const auto batchFunc = [](const Matrix<value_t> &_matrix1, const Matrix<value_t> &_matrix2, Matrix<value_t> &_output, const MatrixShape &_matrix1Shape, const MatrixShape &_matrix2Shape, const GMath::size_t _rowStart, const GMath::size_t _rowEnd) {
			Matrix<value_t> transpose = _matrix2;
			transpose.Transpose();

      for (GMath::size_t __row = _rowStart; __row < _rowEnd; __row++) {
        for (GMath::size_t __column = 0; __column < _matrix2Shape.Columns; __column++) {
          for (GMath::size_t __index = 0; __index < _matrix1Shape.Columns; __index++) {
            _output[__row][__column] += _matrix1[__row][__index] * transpose[__column][__index];
            // _output[__row][__column] += _matrix1[__row][__index] * _matrix2[__index][__column];
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
  Matrix operator/(Matrix<value_t> _matrix) const {
		_matrix.Inverse();
    return *this * _matrix;
  }

  /*
   * Get an array of a row in the matrix.
   */
  [[nodiscard]]
  MatrixRow<value_t> &operator[](const GMath::size_t _n) { return m_rows[_n]; }

  /*
   * Get an array of a row in the matrix.
   */
  [[nodiscard]]
  const MatrixRow<value_t> &operator[](const GMath::size_t _n) const { return m_rows[_n]; }
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
  for (GMath::size_t __row = 0; __row < shape.Rows; __row++) {
		_stream << '\t' << "[ ";
		for (GMath::size_t __column = 0; __column < shape.Columns; __column++) {
			_stream << _matrix[__row][__column];

			if (__column < shape.Columns - 1) {
				_stream << ' ';
			}
		}
		_stream << " ]\n";
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
