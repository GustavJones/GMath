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

  std::cout << std::endl;
  std::cout << "Slice: " << std::endl;
  std::cout << m7.Slice(0, 0, 2, 2) << std::endl;

  std::cout << "Transpose: " << std::endl;
  std::cout << m3 << std::endl;
  std::cout << m3.Transpose() << std::endl;

  std::cout << "Determinant: " << std::endl;
  GMath::Matrix3<GMath::double_t> determinant = {
    {2, 4, 7},
    {-4, -8, 3},
    {-1, 3, 0}
  };
  std::cout << determinant << std::endl;
  std::cout << determinant.Determinant() << std::endl;

  std::cout << m3.InsertColumn({1, 2, 3}, 0);
  std::cout << std::endl;
  std::cout << m3.InsertRow({1, 2, 3}, 0);

  return 0;
}
