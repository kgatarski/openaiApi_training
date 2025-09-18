# CMake configuration for unit tests

enable_testing()
find_package(GTest REQUIRED)

add_executable(myshared_ptr_test myshared_ptr_test.cpp)
target_link_libraries(myshared_ptr_test GTest::gtest GTest::gtest_main pthread)

add_executable(myunique_ptr_test myunique_ptr_test.cpp)
target_link_libraries(myunique_ptr_test GTest::gtest GTest::gtest_main pthread)

add_test(NAME myshared_ptr_test COMMAND myshared_ptr_test)
add_test(NAME myunique_ptr_test COMMAND myunique_ptr_test)
