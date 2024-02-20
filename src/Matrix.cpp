#include "GMath/Matrix.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace GMath {
Matrix::Matrix() : std::vector<std::vector<double>>(1) { (*this)[0].resize(1); }

Matrix::Matrix(int rows, int columns) : std::vector<std::vector<double>>(rows) {

  for (auto &vector : *this) {
    vector.resize(columns);
  }
}

int Matrix::Rows() { return this->size(); }

int Matrix::Columns() { return (*this)[0].size(); }

void Matrix::Print() {
  for (auto &row : *this) {
    std::cout << "{ ";

    for (auto &column : row) {
      std::cout << column << " ";
    }

    std::cout << "}\n";
  }
}

Matrix Matrix::Identity() {
  if (Rows() != Columns()) {
    throw std::invalid_argument("Matrix not square");
  }

  for (auto &row : *this) {
    for (auto &column : row) {
      column = 0;
    }
  }

  for (int i = 0; i < Rows(); i++) {
    (*this)[i][i] = 1;
  }

  return *this;
}

Matrix Matrix::Inverse() {
  if (Rows() != Columns()) {
    throw std::invalid_argument("Matrix not square");
  }

  Matrix output(Rows(), Columns());
  output.Identity();

  double factor;

  // Every Row
  for (int rowIndex = 0; rowIndex < Rows(); rowIndex++) {

    // In Matrix[i][i] = 1
    factor = (*this)[rowIndex][rowIndex];
    for (int columnIndex = 0; columnIndex < Columns(); columnIndex++) {
      // Input Matrix
      (*this)[rowIndex][columnIndex] = (*this)[rowIndex][columnIndex] / factor;

      // Output Matrix
      output[rowIndex][columnIndex] = output[rowIndex][columnIndex] / factor;
    }

    // R(RowIndex) * Row[0] - Row -> Row
    for (int i = rowIndex + 1; i < Rows(); i++) {
      factor = (*this)[i][rowIndex];

      for (int j = 0; j < Columns(); j++) {
        // Input Matrix
        (*this)[i][j] = (*this)[i][j] - ((*this)[rowIndex][j] * factor);

        // Output Matrix
        output[i][j] = output[i][j] - (output[rowIndex][j] * factor);
      }
    }

    if (rowIndex != 0) {
      for (int i = rowIndex - 1; i >= 0; i--) {
        factor = (*this)[i][rowIndex];

        for (int j = 0; j < Columns(); j++) {
          // Input Matrix
          (*this)[i][j] = (*this)[i][j] - ((*this)[rowIndex][j] * factor);

          // Output Matrix
          output[i][j] = output[i][j] - (output[rowIndex][j] * factor);
        }
      }
    }
  }

  *this = output;

  return *this;
}

Matrix Matrix::Round() {
  for (int i = 0; i < Rows(); i++) {
    for (int j = 0; j < Columns(); j++) {
      (*this)[i][j] = round((*this)[i][j]);
    }
  }

  return *this;
}

Matrix Matrix::operator*(Matrix multiply_matrix) {
  if (Columns() != multiply_matrix.Rows()) {
    throw std::invalid_argument("Multiply Matrix not the right size");
  }

  Matrix output(Rows(), multiply_matrix.Columns());
  double indexValue = 0;

  // Loop through every output index
  for (int rowIndex = 0; rowIndex < output.Rows(); rowIndex++) {
    for (int columnIndex = 0; columnIndex < output.Columns(); columnIndex++) {
      indexValue = 0;
      for (int i = 0; i < Columns(); i++) {
        indexValue += (*this)[rowIndex][i] * multiply_matrix[i][columnIndex];
      }

      output[rowIndex][columnIndex] = indexValue;
    }
  }

  return output;
}

Matrix Matrix::operator+(Matrix addition_matrix) {
  if (Columns() != addition_matrix.Columns() ||
      Rows() != addition_matrix.Rows()) {
    throw std::invalid_argument("Addition Matrix not the right size");
  }

  Matrix output(Rows(), Columns());

  for (int i = 0; i < Rows(); i++) {
    for (int j = 0; j < Columns(); j++) {
      output[i][j] = (*this)[i][j] + addition_matrix[i][j];
    }
  }

  return output;
}

Matrix Matrix::operator-(Matrix subtraction_matrix) {
  if (Columns() != subtraction_matrix.Columns() ||
      Rows() != subtraction_matrix.Rows()) {
    throw std::invalid_argument("Addition Matrix not the right size");
  }

  Matrix output(Rows(), Columns());

  for (int i = 0; i < Rows(); i++) {
    for (int j = 0; j < Columns(); j++) {
      output[i][j] = (*this)[i][j] - subtraction_matrix[i][j];
    }
  }

  return output;
}

} // namespace GMath
