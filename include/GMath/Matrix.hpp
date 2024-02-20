#pragma once
#include "LibraryAPI.hpp"
#include <vector>

namespace GMath {
class GMATH_API Matrix : public std::vector<std::vector<double>> {
public:
  Matrix();
  Matrix(int rows, int columns);
  int Rows();
  int Columns();
  void Print();

  Matrix Identity();
  Matrix Inverse();
  Matrix Round();

  Matrix operator*(Matrix multiply_matrix);
  Matrix operator+(Matrix addition_matrix);
  Matrix operator-(Matrix subtraction_matrix);

  Matrix(Matrix &&) = default;
  Matrix(const Matrix &) = default;
  Matrix &operator=(Matrix &&) = default;
  Matrix &operator=(const Matrix &) = default;
};
} // namespace GMath
