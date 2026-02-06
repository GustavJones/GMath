#pragma once
#include "GMath/Matrix.hpp"

namespace GMath {
  template<typename value_t>
  class Matrix3 : public Matrix<value_t> {
  private:
    void Reshape(const MatrixShape &_shape) override {
      Matrix<value_t>::Reshape(_shape);
    };

  public:
    Matrix3() : Matrix<value_t>(3, 3) {};
    Matrix3(const std::initializer_list<DynamicArray<value_t>> &_list) : Matrix<value_t>(_list) {
      auto shape = Matrix<value_t>::Shape();
      if (shape != MatrixShape(3, 3)) {
        throw std::runtime_error("Invalid matrix shape for Matrix3.");
      }
    }

    Matrix3(Matrix3 &&) = default;
    Matrix3(const Matrix3 &) = default;
    Matrix3 &operator=(Matrix3 &&) = default;
    Matrix3 &operator=(const Matrix3 &) = default;
    ~Matrix3() = default;
  };
}
