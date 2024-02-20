#pragma once
#include "LibraryAPI.hpp"
#include "Matrix.hpp"

namespace GMath {
class GMATH_API Matrix2 : public Matrix {
public:
  Matrix2();
  Matrix2(Matrix matrix);
};
} // namespace GMath
