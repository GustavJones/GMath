#pragma once
#include "LibraryAPI.hpp"

namespace GMath {
class GMATH_API Fraction {
public:
  int numerator;
  int denominator;

  Fraction(double value);
  Fraction();

  operator double() const;

  void Print();
};
} // namespace GMath
