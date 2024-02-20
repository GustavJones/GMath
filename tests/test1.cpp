#include "GMath/Fraction.hpp"
#include "GMath/Matrix.hpp"
#include "GMath/Matrix2.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  GMath::Matrix test(5, 2);

  /*
   * {
   *    {5, 6},
   *    {4, 9},
   *    {2, 8},
   *    {1, 3},
   *    {7, 0}
   * }
   *
   * */

  test[0][0] = 5;
  test[0][1] = 6;
  test[1][0] = 4;
  test[1][1] = 9;
  test[2][0] = 2;
  test[2][1] = 8;
  test[3][0] = 1;
  test[3][1] = 3;
  test[4][0] = 7;
  test[4][1] = 0;

  test.Print();

  std::cout << '\n';

  GMath::Matrix2 test2;

  /*
   * {
   *    {3, 6},
   *    {9, 8}
   * }
   * */

  test2[0][0] = 3;
  test2[0][1] = 6;
  test2[1][0] = 9;
  test2[1][1] = 8;

  test2.Print();

  std::cout << '\n';

  GMath::Matrix test3 = test * test2;
  test3.Print();

  std::cout << "\n----------------------------------\n\n";

  GMath::Fraction fraction1(1.059090416043312e-7);
  fraction1.Print();

  return 0;
}
