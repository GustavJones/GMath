# GMath

## Table of content

1. [Description](#description)
2. [Installation](#installation)
3. [Usage](#usage)

## Description

This is a simple header only template library for math related objects. At the moment it adds Matrix objects with different methods.

## Installation

To use this as a library simply download a copy from this repo and add it to your CMake project.

- Get a copy of the library:

  Use `git clone https://github.com/GustavJones/GMath.git`
  or `git submodule add https://github.com/GustavJones/GMath.git GMath` (recommended)
  or Download the ZIP archieve directly from Github.

- Add the library to the CMake project to build at compile time (optional):

  Use `add_subdirectory()` in your CMake project and include GNeuro directory.

- Include and Link the library files to your project:

  Include the library headers with `target_include_directories()` to the `GMath/include` directory.

## Usage

> [!NOTE]
> This library supports templates but I recommend that you use the `GMath::double_t` type.

```c++
  // Create a matrix object
  GMath::Matrix<GMath::double_t> m1;
  GMath::Matrix<GMath::double_t> m2(2, 2);
  GMath::Matrix<GMath::double_t> m3 {
    {2, 3},
    {5, 7}
  };

  GMath::Matrix2<GMath::double_t> m4;
  GMath::Matrix3<GMath::double_t> m5;

  // Arithmetic
  std::cout << m2 + m3;
  std::cout << m2 - m3;
  std::cout << m2 * m3;
  std::cout << m2 / m3;

  // Special operations
  std::cout << m1.Inverse();
  std::cout << m1.Round();

  // Mutator methods
  m2.Identity();
  m2.Zero();
  m2.Reshape({2, 3});
```
