#include "GMath/Fraction.hpp"
#include <cmath>
#include <iostream>
#include <numeric>

namespace GMath {
Fraction::Fraction(double value) {
  int wholeNumber = static_cast<int>(value);
  value -= wholeNumber;

  // Initialize numerator and denominator
  numerator = static_cast<int>(
      value * pow(10, std::string(std::to_string(value)).size() - 2));
  denominator = pow(10, std::string(std::to_string(value)).size() - 2);

  // Simplify the fraction
  int gcd = std::gcd(numerator, denominator);
  numerator /= gcd;
  denominator /= gcd;

  // Add the whole number part to the numerator
  numerator += wholeNumber * denominator;
}
Fraction::Fraction() : numerator(0), denominator(0) {}

Fraction::operator double() const { return ((double)numerator / denominator); }

void Fraction::Print() { std::cout << numerator << '/' << denominator << '\n'; }
} // namespace GMath
