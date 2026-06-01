if(GMATH_BUILD_TESTS)
  message(STATUS "Building GMath tests")
  add_executable(test "src/GMath/test.cpp")
  target_include_directories(test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
  set_target_properties(test PROPERTIES 
    CXX_STANDARD 11 
    CXX_STANDARD_REQUIRED TRUE
  )
else()
  message(STATUS "Skipping GMath tests")
endif()
