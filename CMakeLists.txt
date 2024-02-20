cmake_minimum_required(VERSION 3.5)
project(GMath)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(BUILD_SHARED_LIBS True)
set(BUILD_TESTS True)

file(GLOB SOURCES "src/*.cpp")

if(BUILD_SHARED_LIBS)
  add_library(${PROJECT_NAME} SHARED ${SOURCES})
else()
  add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_compile_definitions(${PROJECT_NAME} PRIVATE GMATH_BUILD_DLL)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GMATH_WINDOWS)
endif()

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
