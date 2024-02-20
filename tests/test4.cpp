#include "GMath/Fraction.hpp"
#include "GMath/Matrix2.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  GMath::Matrix2 test;
  test[0] = {5, 6};
  test[1] = {4, 9};

  GMath::Matrix test2(4, 4);
  test2[0] = {3, 6, 7, 3};
  test2[1] = {9, 8, 0, 4};
  test2[2] = {4, 5, 2, 3};
  test2[3] = {5, 2, 1, 9};

  std::cout << "Input" << '\n';

  test2.Print();

  std::cout << "Output" << '\n';

  test.Inverse();
  test.Print();

  test2.Inverse();
  test2.Print();

  for (auto row : test2) {
    for (auto item : row) {
      GMath::Fraction frac(item);
      frac.Print();
    }
  }

  return 0;
}
