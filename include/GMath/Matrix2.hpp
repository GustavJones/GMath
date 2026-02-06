#pragma once
#include "GMath/Matrix.hpp"
#include <stdexcept>

namespace GMath {
  template<typename value_t>
  class Matrix2 : public Matrix<value_t> {
  private:
    void Reshape(const MatrixShape &_shape) override {
      Matrix<value_t>::Reshape(_shape);
    };

  public:
    Matrix2() : Matrix<value_t>(2, 2) {};
    Matrix2(const std::initializer_list<DynamicArray<value_t>> &_list) : Matrix<value_t>(_list) {
      auto shape = Matrix<value_t>::Shape();
      if (shape != MatrixShape(2, 2)) {
        throw std::runtime_error("Invalid matrix shape for Matrix2.");
      }
    }

    Matrix2(Matrix2 &&) = default;
    Matrix2(const Matrix2 &) = default;
    Matrix2 &operator=(Matrix2 &&) = default;
    Matrix2 &operator=(const Matrix2 &) = default;
    ~Matrix2() = default;
  };
}
