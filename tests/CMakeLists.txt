file(GLOB TEST1_SOURCES "test1.cpp")
add_executable(TEST1 ${TEST1_SOURCES})
target_include_directories(TEST1 PRIVATE ../include)
target_link_libraries(TEST1 PRIVATE GMath)

file(GLOB TEST2_SOURCES "test2.cpp")
add_executable(TEST2 ${TEST2_SOURCES})
target_include_directories(TEST2 PRIVATE ../include)
target_link_libraries(TEST2 PRIVATE GMath)

file(GLOB TEST3_SOURCES "test3.cpp")
add_executable(TEST3 ${TEST3_SOURCES})
target_include_directories(TEST3 PRIVATE ../include)
target_link_libraries(TEST3 PRIVATE GMath)

file(GLOB TEST4_SOURCES "test4.cpp")
add_executable(TEST4 ${TEST4_SOURCES})
target_include_directories(TEST4 PRIVATE ../include)
target_link_libraries(TEST4 PRIVATE GMath)
