#include "GMath/Matrix2.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  GMath::Matrix2 test;
  test[0] = {5, 6};
  test[1] = {4, 9};

  GMath::Matrix test2(3, 3);
  test2[0] = {3, 6, 7};
  test2[1] = {9, 8, 0};
  test2[2] = {4, 5, 2};

  std::cout << "Input" << '\n';

  test2.Print();

  std::cout << "Output" << '\n';

  GMath::Matrix test3 = test2;
  GMath::Matrix test4;

  test2.Inverse();
  test2.Print();
  test3.Print();

  test4 = (test3 * test2);
  test4.Round();
  std::cout << "Check" << '\n';
  test4.Print();

  return 0;
}
