#include "GMath/Matrix.hpp"
#include "GMath/Matrix2.hpp"
#include "GMath/Matrix3.hpp"
#include "GMath/Types.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  GMath::Matrix<GMath::double_t> m1;
  GMath::Matrix<GMath::double_t> m2;

  m1 = {
		{1, 2}, 
		{3, 4}
	};

  m2 = {
		{-2, 3}, 
		{4, -5}
	};

  auto m3 = m1 + m2;
  auto m4 = m3 - m2;
  auto m5 = m3 * m4;

  std::cout << m3 << std::endl;
  std::cout << m4 << std::endl;
  std::cout << std::endl;

	std::cout << "Transpose:" << std::endl;
	std::cout << m1 << std::endl;
	m1.Transpose();
	std::cout << m1 << std::endl;

	std::cout << "Inverse:" << std::endl;
  std::cout << m5 << std::endl;
	m5.Inverse();
  std::cout << m5 << std::endl;

	std::cout << "Round:" << std::endl;
	m5 = m5 * 10;
	m5.Round();
  std::cout << m5 << std::endl;
	std::cout << "Zero:" << std::endl;
	m5.Zero();
  std::cout << m5 << std::endl;

	std::cout << "Matrix divide:" << std::endl;
  std::cout << m5 / m4 << std::endl;

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

	std::cout << "Factor multiply:" << std::endl;
  std::cout << 6.0 * m6 << std::endl;
  std::cout << 7.0 * m7 << std::endl;

  std::cout << std::endl;
  std::cout << "Slice: " << std::endl;
	m7.Slice(1, 0, {2, 2});
  std::cout << m7 << std::endl;

  std::cout << "Transpose: " << std::endl;
  std::cout << m3 << std::endl;
	m3.Transpose();
  std::cout << m3 << std::endl;

  std::cout << "Determinant: " << std::endl;
  GMath::Matrix3<GMath::double_t> determinant = {
    {2, 4, 7},
    {-4, -8, 3},
    {-1, 3, 0}
  };
  std::cout << determinant << std::endl;
  std::cout << determinant.Determinant() << std::endl;

	std::cout << "Insert:" << std::endl;
	m3.InsertColumn({1, 2}, 0);
  std::cout << m3 << std::endl;
  std::cout << std::endl;
	m3.InsertRow({1, 2, 3}, 0);
  std::cout << m3 << std::endl;

  return 0;
}
