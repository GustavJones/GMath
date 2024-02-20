#include "GMath/Matrix.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  GMath::Matrix test(3, 3);
  test[0] = {5, 6, 7};
  test[1] = {4, 9, 3};
  test[2] = {2, 1, 9};

  GMath::Matrix test1(3, 3);
  test1[0] = {1, 0, 0};
  test1[1] = {0, 1, 0};
  test1[2] = {0, 0, 1};

  GMath::Matrix test2(3, 3);
  test2[0] = {3, 6, 9};
  test2[1] = {9, 8, 2};
  test2[2] = {4, 7, 0};

  test.Print();
  test2.Print();

  GMath::Matrix test3 = test * test2;
  GMath::Matrix test4 = test + test2;
  GMath::Matrix test5 = test - test2;
  std::cout << '\n';
  test3.Print();
  std::cout << '\n';
  test4.Print();
  std::cout << '\n';
  test5.Print();
  std::cout << '\n';
  return 0;
}
