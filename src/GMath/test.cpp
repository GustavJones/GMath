#include "GMath/Matrix.hpp"
#include "GMath/Matrix2.hpp"
#include "GMath/Matrix3.hpp"
#include "GMath/Types.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  GMath::Matrix<GMath::double_t> m1;
  GMath::Matrix<GMath::double_t> m2;

  m1 = {{1, 2}, {3, 4}};

  m2 = {{-2, 3}, {4, -5}};

  auto m3 = m1 + m2;
  auto m4 = m3 - m2;
  auto m5 = m3 * m4;

  std::cout << m3;
  std::cout << m4;
  std::cout << std::endl;
  std::cout << m5;
  std::cout << m5.Inverse();

  std::cout << m5 / m4;

  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;

  GMath::Matrix2<GMath::double_t> m6;
  GMath::Matrix3<GMath::double_t> m7;

  m6 = {
    {1, 0},
    {0, 1}
  };

  m7 = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
  };

  std::cout << 6 * m6;
  std::cout << 7 * m7;

  return 0;
}
