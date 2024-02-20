#include "GMath/Matrix2.hpp"
#include <stdexcept>

namespace GMath {
Matrix2::Matrix2() : Matrix(2, 2) {}

Matrix2::Matrix2(Matrix matrix) : GMath::Matrix(2, 2) {
  if (matrix.Rows() != 2 || matrix.Columns() != 2) {
    throw std::invalid_argument("Invalid Matrix size for cast");
  }

  matrix.Print();

  (*this)[0] = matrix[0];
  (*this)[1] = matrix[1];
}
} // namespace GMath
